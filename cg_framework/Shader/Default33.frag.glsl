#version 330 core

// Input variables from the previous shader stage.
in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

// Uniform variable: passed to the shader from CPU side.
uniform vec3 hsv_user_input;
uniform vec3 lightPos;
uniform vec4 camPos;

// Output variable which determines the final color of the fragment.
out vec4 FragColor;

float maxFloat(float a, float b, float c)
{
	float mx;
	mx = max(a, b);
	return max(mx, c);
}

float minFloat(float a, float b, float c)
{
	float mn;
	mn = min(a, b);
	return min(mn, c);
}

vec3 RGB2HSV(vec3 rgb)
{
	vec3 hsv;
	rgb.x = rgb.x;
	rgb.y = rgb.y;
	rgb.z = rgb.z;
	float max;
	max = maxFloat(rgb.x, rgb.y, rgb.z);
	float min;
	min = minFloat(rgb.x, rgb.y, rgb.z);
	float h;
	float s;
	float v;
	float fstdiff = max - min;
	
	if(max == min)
	{
		h = 0;
	}
	else
	{
		float i = 0;
		float secdiff = 0;
		
		if(max == rgb.x)
		{
			secdiff = rgb.y - rgb.z;
		}
		else if(max == rgb.y)
		{
			i = 2; 
			secdiff = rgb.z - rgb.x;
		}
		else if(max == rgb.z)
		{
			i = 4; 
			secdiff = rgb.x - rgb.y;
		}
		
		h = 60 * (i + (secdiff/fstdiff));
		if(h < 0)
		{
			h += 360;
		}
	}
	
	if(max == 0)
	{
		s = 0;
	}
	else
	{
		s = (fstdiff / max);
	}
	
	v = max;
	
	hsv.x = h;
	hsv.y = s;
	hsv.z = v;
	return hsv;
}

vec3 HSV2RGB(vec3 hsv)
{
   int hi;
   float f;
   float p;
   float q;
   float t;
   float V = hsv.z;

   vec3 RGB;


   hi = int((hsv.x/60));
   f = ((hsv.x/60)- hi);
   p = hsv.z * (1 - hsv.y);
   q = hsv.z * (1 - hsv.y * f);
   t = hsv.z * (1 - hsv.y * (1 - f));
   switch(hi)
   {
        case 0 :  RGB.x = V;
                  RGB.y = t;
                  RGB.z = p;
                  return RGB;
                  break;
        case 1 : 
                  RGB.x = q;
                  RGB.y = V;
                  RGB.z = p;
                  return RGB;
                  break;
        case 2 : 
                  RGB.x = p;
                  RGB.y = V;
                  RGB.z = t;
                  return RGB;
                  break;
        case 3 : 
                  RGB.x = p;
                  RGB.y = q;
                  RGB.z = V;
                  return RGB;
                  break;
        case 4 :
                  RGB.x = t;
                  RGB.y = p;
                  RGB.z = V;
                  return RGB;
                  break;
        case 5 : 
                  RGB.x = V;
                  RGB.y = p;
                  RGB.z = q;
                  return RGB;
                  break;
        case 6 :
                  RGB.x = V;
                  RGB.y = t;
                  RGB.z = p;
                  return RGB;
                  break;
   }
}

void main()
{
   vec4 color = VertColor;
   vec3 hsv;
   vec3 rgb;
   
   //To ensure the precondition of RGB2HSV
   rgb.x = color.x;
   rgb.y = color.y;
   rgb.z = color.z;
   
   hsv = RGB2HSV(rgb);
   hsv = hsv + hsv_user_input;
   /*
   hsv.x = hsv.x + hsv_user_input.x;
   hsv.y = hsv.y + hsv_user_input.y;
   hsv.z = hsv.z + hsv_user_input.z;*/
   
   rgb = HSV2RGB(hsv);
   /*
   color.x = rgb.x;
   color.y = rgb.y;
   color.z = rgb.z;
   */
   FragColor = vec4(rgb.x, rgb.y, rgb.z, color.w);	
   float x = dot((camPos - VertPosition), VertNormal);
   if(x <= 0)
   {
		//discard;
   }
}