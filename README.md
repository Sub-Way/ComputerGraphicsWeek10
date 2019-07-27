# ComputerGraphicsWeek10

## 실습 10주차. Viewing

예제 코드의 마우스처럼 키보드를 이용해서 카메라를 이동시키시오.  
### 1.	‘W’: 카메라를 앞으로 이동 (20점)

~~~
if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		static int proj = 1;
		gl_world.camera_.SetMode(proj);
	}

void GL_VIEW::SetMode(int proj)
{
	dolly_.z += proj;
}
~~~  
  
![image](https://user-images.githubusercontent.com/22046757/61995502-7684bd00-b0c4-11e9-8fc7-3d1dd174d522.png)

### 2.	‘S’: 카메라를 뒤로 이동 (20점)

~~~
else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		static int proj = -1;
		gl_world.camera_.SetMode(proj);
	}

void GL_VIEW::SetMode(int proj)
{
	dolly_.z += proj;
}
~~~
![image](https://user-images.githubusercontent.com/22046757/61995515-94522200-b0c4-11e9-955e-dc664114f672.png)


### 3.	‘D’: 카메라를 오른쪽으로 이동 (20점)
~~~
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		static int proj = -1;
		gl_world.camera_.SetMode_2(proj);
	}

void GL_VIEW::SetMode_2(int proj)
{
	dolly_.x += proj;
}
~~~
![image](https://user-images.githubusercontent.com/22046757/61995523-b8156800-b0c4-11e9-8fb7-28983859d88f.png)


### 4.	‘A’: 카메라를 왼쪽으로 이동 (20점)
~~~
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		static int proj = 1;
		gl_world.camera_.SetMode_2(proj);
	}

void GL_VIEW::SetMode_2(int proj)
{
	dolly_.x += proj;
}
~~~
![image](https://user-images.githubusercontent.com/22046757/61995524-b9df2b80-b0c4-11e9-8b80-20acd674d974.png)

### 5.	‘Q/E’: 카메라를 중심으로 왼쪽/오른쪽으로 회전 (20점)
~~~
else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		static float proj = -0.1;
		glm::vec3 c(0.0, 1.0, 0.0);
		gl_world.camera_.SetMode_3(proj, c);
	}

	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		static float proj = 0.1;
		glm::vec3 c(0.0, 1.0, 0.0);
		gl_world.camera_.SetMode_3(proj, c);
	}

void GL_VIEW::SetMode_3(float v, glm::vec3 vec)
{
	SetCenterOfRotation(vec);

	vec = glm::vec3(0.0f, 1.0f, 0.0f);
	projection_ = projection_ * glm::rotate(v, vec);
}
~~~ 
![image](https://user-images.githubusercontent.com/22046757/61995529-c19ed000-b0c4-11e9-8560-ed2c20fb0fe2.png)
![image](https://user-images.githubusercontent.com/22046757/61995531-c4012a00-b0c4-11e9-9a66-5979b30516ec.png) 



 

