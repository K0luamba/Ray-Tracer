#version 330

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define float4x4 mat4
#define float3x3 mat3

const float EPSILON = 1e-3f;
const float MAX_DIST = 1e+30; //ограничение дальности просмотра
const int MAX_REFLECTION_DEPTH = 4;

//из вершинного шейдера
in float2 fragmentTexCoord;
//единственное, что возвращаем явно
layout(location = 0) out vec4 fragColor;

uniform int g_screenWidth;
uniform int g_screenHeight;
//получаем из main.cpp, меняется в зависимости от положения камеры
uniform float4x4 g_rayMatrix;
uniform float g_mirrorPos;
uniform float g_lightPower;
uniform int g_AA;
uniform bool g_firstTime;
uniform float4 g_bgColor = float4(0.41, 0.72, 0.90, 1);

//корректирует цвета вышедшие за пределы [0,1]
float4 check_color(float4 c)
{
    float4 res;
    for (int i = 0; i < 4; i++)
    {
        if (c[i] < 0) 
        {
            res[i] = 0;
        }
        else if (c[i] > 1) 
        {
            res[i] = 1;
        }
        else res[i] = c[i];
    }
    return res;
}

float max(float x1, float x2)
{ 
    if (x1 >= x2)
        return x1;
    else
        return x2;
}

//=========структуры и классы для шейдеров==========//

struct Ray
{
   float3 pos; //по канону задания
   float3 dir;
};

struct Light
{
    float3 position;
    float4 intensivity; //1 - сохранение дефолтного цвета, <1 - менее яркий, >1 - более яркий
};

struct Sphere
{
    float4 color;
    float3 position; 
    float radius;
    float albedo; 
    float spec_factor; 
    float4 reflection; //0 - нет отражения; 1 - максимальное отражение (принимает весь цвет следующего объекта)
};

    //(cделано на основе статьи из Хабра)
    bool RaySphereIntersection(Ray ray, inout float t, float3 position, float radius) 
    {
        float3 L = position - ray.pos; //луч в центр
        float tca = dot(L, ray.dir); 
        if (tca < 0) //если сфера сзади
        { 
            return false;
        }
        else {
            float d1 = (dot(L,L)) - (tca * tca);
            float d = sqrt(d1);
            if (d > radius) //если при попытке спроецирования центра вышли за границу сферы
            {
                return false;
            }
            else {
                float thc = sqrt((radius * radius) - d1);
                float t0 = tca - thc;
                t = t0; //первое пересечение со сферой сохраняем по ссылке
                return true;
            }
        }
    }

    float3 get_sphere_normal(float3 point, float3 position) 
    {
        return normalize(point - position); //просто вектор из центра к точке на поверхности
    }

struct Plane
{
    float4 color;
    float3 position; //подходит любая точка плоскости
    float3 normal;
    float albedo; 
    float spec_factor; 
    float4 reflection;
};

    bool RayPlaneIntersection(Ray ray, inout float t, float3 position, float3 plane_normal) 
    {
        float denom = dot(ray.dir, plane_normal);

        if (abs(denom) >= 0.00001f) //если направение взгляда почти параллельно сфере
        {
            float t0 = dot((position - ray.pos), plane_normal) / denom;
            t = t0;
            return t >= 0; //если <0, значит луч пересекает сферу своим обратным направлением
        }
        return false;
    }

struct Disk
{
    float4 color;
    float3 position;
    float radius;
    float3 normal;
    float albedo; 
    float spec_factor; 
    float4 reflection; 
};

    bool RayDiskIntersection(Ray ray, inout float t, float3 position, float3 disk_normal, float radius) 
    {
        if (RayPlaneIntersection(ray, t, position, disk_normal))
        {
            float3 p = ray.pos + (ray.dir * t); //место пересечения с плоскостью, в которой лежит сфера
            float3 v = p - position; 
            float rad1 = dot(v,v);
            return (sqrt(rad1) <= radius);
        }
        return false;
    }

float3 reflect(float3 into, float3 normal) 
{
    float3 res = into - normal*2.0*dot(into,normal);
    return res;
}


//задает направление первичного луча из "глаза"
float3 EyeRayDir(float x, float y, float w, float h)
{
	float fov = 3.141592654f/(2.0f); //угол зрения
    float3 ray_dir; 
  
	ray_dir.x = x+0.5f - (w/2.0f);
	ray_dir.y = y+0.5f - (h/2.0f);
	ray_dir.z = -(w)/tan(fov/2.0f);
	
    return normalize(ray_dir);
}

//тут определяем "ячейки" для фигур и источников света
const int num_s = 11;
Sphere spheres[num_s];
Light lights[3]; 
Plane planes[2];
Disk disks[4];

//определение всех объектов сцены
void setPrimitives()
{
    //источники света
    lights[0].position = float3(0,8,-15);
    lights[0].intensivity =  float4(g_lightPower, g_lightPower, g_lightPower, 1);
    lights[1].position = float3(10,12,0);
    lights[1].intensivity =  float4(g_lightPower, g_lightPower, g_lightPower, 1);
    lights[2].position = float3(0,8,15);
    lights[2].intensivity =  float4(g_lightPower, g_lightPower, g_lightPower, 1);
    //сферы
    if (g_firstTime == false)
    {
        spheres[0].position = float3(0,-5,7);
        spheres[0].color = float4(0.55, 0.55, 0.55, 1);
        spheres[1].position = float3(0,-5,-7);
        spheres[1].color = float4(0.1, 0.55, 0.1, 1);
        spheres[2].position = float3(1,-5,-8.8);
        spheres[2].color = float4(0.6, 0.1, 0.1, 1);
        spheres[3].position = float3(-1,-5,-8.8);
        spheres[3].color = float4(0.1, 0.12, 0.48, 1);
        spheres[4].position = float3(0,-5,-10.6);
        spheres[4].color = float4(0.06, 0.06, 0.06, 1);
        spheres[5].position = float3(-2,-5,-10.6);
        spheres[5].color = float4(0.6, 0.30, 0.1, 1);
        spheres[6].position = float3(2,-5,-10.6);
        spheres[6].color = float4(0.3, 0.1, 0.5, 1);
        spheres[7].position = float3(3,-5,-12.4);
        spheres[7].color = float4(0.5, 0.4, 0.05, 1);
        spheres[8].position = float3(1,-5,-12.4);
        spheres[8].color = float4(0.45, 0.05, 0.12, 1);
        spheres[9].position = float3(-1,-5,-12.4);
        spheres[9].color = float4(0.1, 0.45, 0.45, 1);
        spheres[10].position = float3(-3,-5,-12.4);
        spheres[10].color = float4(0.45, 0, 0.45, 1);
        for (int i = 0; i < num_s; i++) //общие характеристики материала ~ слоновая кость
        {
            spheres[i].radius = 1;
            spheres[i].spec_factor = 80;
            spheres[i].albedo = 0.8;
            spheres[i].reflection = float4(0.05, 0.05, 0.05, 1);
        }
    }
    //плоскости
    planes[0].position = float3(0, -6, 0);
    planes[0].normal = float3(0, 1, 0);
    planes[0].color = float4(0, 0.33, 0.33, 1);
    planes[0].spec_factor = 0;
    planes[0].albedo = 0;
    planes[0].reflection = float4(0.0, 0.0, 0.0, 1);
    planes[1].position = float3(0, 0, -23);
    planes[1].normal = float3(0, 0, -1);
    planes[1].color = float4(0.35, 0.22, 0.08, 1);
    planes[1].spec_factor = 0;
    planes[1].albedo = 0;
    planes[1].reflection = float4(0.0, 0.0, 0.0, 1);
    //диски
    if (g_firstTime == false)
    {
        disks[0].position = float3(g_mirrorPos, 1,-22.99);
        disks[0].radius = 6.5;
        disks[0].reflection = float4(0.7, 0.7, 0.7, 1);
        disks[0].normal = float3(0, 0, -1);
        //эти 3 зеркала позволяют с определенной позиции увидеть одинокий шар в сразу 3х цветовых фильтрах (см. скриншот)
        disks[1].position = float3(-4.5, 10,-22.0);
        disks[1].radius = 2;
        disks[1].reflection = float4(0.8, 0.2, 0.2, 1);
        disks[1].normal = float3(-0.25, 0, -1);
        disks[2].position = float3(0, 10,-22.99);
        disks[2].radius = 2;
        disks[2].reflection = float4(0.2, 0.8, 0.2, 1);
        disks[2].normal = float3(0, 0, -1);
        disks[3].position = float3(4.5, 10,-22.0);
        disks[3].radius = 2;
        disks[3].reflection = float4(0.2, 0.2, 0.8, 1);
        disks[3].normal = float3(0.25, 0, -1);
        for (int i = 0; i < 4; i++)
        {
            disks[i].color = float4(0.0, 0.0, 0.0, 1);
            disks[i].spec_factor = 0;
            disks[i].albedo = 0;
        }
    }
}

float4 RayTrace(Ray ray)
{
    float4 pix_color = float4(0.0, 0.0, 0.0, 0.0);
    float3 hit_point;
    float dist_to_obj = 0.0f;   //дистанция до проверяемого объекта 
    float dist_minimal = MAX_DIST;    //дистанция до ближнего объекта
    float4 diffuse_color; 
    float3 normal; 
    float3 position;
    float3 a, b, c; 
    float albedo;
    float spec_factor;
    float4 reflection = float4(1, 1, 1, 1);; //этот коэффициент будем изменять при дальнейших отражениях
    float4 reflection_cur;
    int obj_ident = 0; //для запоминания типа пересеченного объекта 

    for(int r=0; r < MAX_REFLECTION_DEPTH; r++)
    {
        dist_to_obj = 0.0f;
        dist_minimal = MAX_DIST; 
        obj_ident = 0;
        //далее проверяем поочередно все типы фигур сцены
        if (g_firstTime == false)
        {
            for (int k = 0; k < num_s; k++)
            {
                bool hit = RaySphereIntersection(ray, dist_to_obj, spheres[k].position, spheres[k].radius);
                if (hit && dist_to_obj < dist_minimal)
                { 
                    dist_minimal = dist_to_obj;
                    diffuse_color = spheres[k].color;
                    position = spheres[k].position;
                    albedo = spheres[k].albedo;
                    spec_factor = spheres[k].spec_factor;
                    reflection_cur = spheres[k].reflection;
                    obj_ident = 1;
                }
            }
        }
        for (int k = 0; k < 2; k++)
        {
            bool hit = RayPlaneIntersection(ray, dist_to_obj, planes[k].position, planes[k].normal);
            if (hit && dist_to_obj < dist_minimal)
            { 
                dist_minimal = dist_to_obj;
                diffuse_color = planes[k].color;
                position = planes[k].position;
                normal = planes[k].normal;
                albedo = planes[k].albedo;
                spec_factor = planes[k].spec_factor;
                reflection_cur = planes[k].reflection;
                obj_ident = 2;
            }
        }
        if (g_firstTime == false)
        {
            for (int k = 0; k < 4; k++)
            {
                bool hit = RayDiskIntersection(ray, dist_to_obj, disks[k].position, disks[k].normal, disks[k].radius);
                if (hit && dist_to_obj < dist_minimal)
                { 
                    dist_minimal = dist_to_obj;
                    diffuse_color = disks[k].color;
                    position = disks[k].position;
                    normal = disks[k].normal;
                    albedo = disks[k].albedo;
                    spec_factor = disks[k].spec_factor;
                    reflection_cur = disks[k].reflection;
                    obj_ident = 3;
                }
            }
        }

        //если есть пересечение 
        if (obj_ident != 0) 
        {
            hit_point = ray.pos + (ray.dir * dist_minimal); 
            //определение нормали в зависимости от типа
            if (obj_ident == 1)
            {
                normal = get_sphere_normal(hit_point, position);
            }
            float4 ambient = diffuse_color * float4(0.07, 0.07, 0.07, 1); //тень
            float4 diffuse_light_intensivity = float4(0, 0, 0, 0);
            float4 specular_light_intensity = float4(0, 0, 0, 0);
            
            int have_light = 0; //флаг освещенности хотя бы одним источником
            //цикл по всем источникам
            for(int l = 0; l < 3; l++)
            {
                int light_obj_ident = 0; //флаг освещенности этим источником
                float3 light_dir = normalize(lights[l].position - hit_point);
                Ray ray_to_light;
                ray_to_light.dir = light_dir;
                ray_to_light.pos = hit_point + (normal*EPSILON);
                if ((obj_ident == 2 || obj_ident == 3 || obj_ident == 4) && dot(ray.dir, normal) > 0) //этот код убирает баг с освещением обратной стороны плоскости
                {
                    normal = -normal;
                    ray_to_light.pos = hit_point + (normal*EPSILON);
                }
                dist_minimal = length(lights[l].position - hit_point);
                //цикл по всем объектам относительно данного источкника 
                if (g_firstTime == false)
                {
                    for (int k = 0; k < num_s; k++) 
                    {
                        bool hit_by_light = RaySphereIntersection(ray_to_light, dist_to_obj, spheres[k].position, spheres[k].radius);
                        if (hit_by_light && dist_to_obj < dist_minimal)
                        {
                            light_obj_ident = 1;
                            dist_minimal = dist_to_obj;
                        }
                    }
                }
                for (int k = 0; k < 2; k++) 
                {
                    bool hit_by_light = RayPlaneIntersection(ray_to_light, dist_to_obj, planes[k].position, planes[k].normal);
                    if (hit_by_light && dist_to_obj < dist_minimal)
                    {
                        light_obj_ident = 1;
                        dist_minimal = dist_to_obj;
                    }
                }
                if (g_firstTime == false)
                {
                    for (int k = 0; k < 4; k++) 
                    {
                        bool hit_by_light = RayDiskIntersection(ray, dist_to_obj, disks[k].position, disks[k].normal, disks[k].radius);
                        if (hit_by_light && dist_to_obj < dist_minimal)
                        {
                            light_obj_ident = 1;
                            dist_minimal = dist_to_obj;
                        }
                    }
                }

                if (light_obj_ident == 0) //если луч достиг данного источника без пересечений
                {
                    //используем освещение, основанное на модели Фонга
                    specular_light_intensity += pow(max(0.0f, dot(-reflect(-light_dir, normal),ray.dir)), spec_factor) * lights[l].intensivity; 
                    diffuse_light_intensivity += lights[l].intensivity * max(0.0f, dot(light_dir, normal)); 
                    have_light = 1; 
                } 
            }

            if (have_light == 1)
            {
                pix_color += reflection * (diffuse_color * diffuse_light_intensivity + float4(1.0, 1.0, 1.0, 1.0)*specular_light_intensity * albedo); 
            }
            else
            {
                pix_color += reflection * ambient;
            }
        }
        else 
        {   
            return check_color(pix_color + reflection * g_bgColor);
        }
        //определяем дальнейшее движение луча
        if (reflection_cur == float4(0.0, 0.0, 0.0, 1)) 
        {
            return check_color(pix_color);
        }
        reflection *= reflection_cur;
        ray.dir = normalize(reflect(ray.dir, normal));
        ray.pos = hit_point + (normal*EPSILON);
    } 
    return check_color(pix_color);
}

void main(void)
{	
  //параметры, неизменяемые без инициации юзером
  float w = float(g_screenWidth);
  float h = float(g_screenHeight);
  
  setPrimitives();

  //координаты текущего пикселя меняются по ходу похождения картинки
  float x = fragmentTexCoord.x*w; 
  float y = fragmentTexCoord.y*h;
  
  //вычисляем точное направление первичного луча (проход по пикселям)
  Ray ray;
  ray.pos = float3(0,0,0);
  ray.dir = EyeRayDir(x,y,w,h); 
  //по сути преобразование из "системы отсчета глаза" в глобальную
  ray.pos = (g_rayMatrix*float4(ray.pos,1)).xyz; //по умолчанию (0,0,5)
  ray.dir = float3x3(g_rayMatrix)*ray.dir;
  if (g_firstTime)
  {
    fragColor = RayTrace(ray);
  }
  else
  {
      if (g_AA == -1)
      {
          fragColor = RayTrace(ray);
          return;
      }
      else
      //анти-алиасинг по 4ем лучам на пиксель
      {
          ray.dir = EyeRayDir(x + 0.25,y + 0.25,w,h);
          ray.dir = float3x3(g_rayMatrix)*ray.dir;
          float4 fragColor1 = RayTrace(ray);

          ray.dir = EyeRayDir(x + 0.25,y - 0.25,w,h); 
          ray.dir = float3x3(g_rayMatrix)*ray.dir;
          float4 fragColor2 = RayTrace(ray);

          ray.dir = EyeRayDir(x - 0.25,y - 0.25,w,h); 
          ray.dir = float3x3(g_rayMatrix)*ray.dir;
          float4 fragColor3 = RayTrace(ray);

          ray.dir = EyeRayDir(x - 0.25,y + 0.25,w,h); 
          ray.dir = float3x3(g_rayMatrix)*ray.dir;
          float4 fragColor4 = RayTrace(ray);

          fragColor = (fragColor1 + fragColor2 + fragColor3 + fragColor4)/4;
      }
  }
    
  return;
}


