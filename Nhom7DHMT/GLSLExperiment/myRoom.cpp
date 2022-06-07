//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include "math.h"

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/


point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;


GLint matrix_loc;

mat4 projection;
GLuint projection_loc;

mat4 view;
GLuint view_loc;
GLfloat mauAnhSang = 1.0;


void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(0.0, 1.0, 1.0, 1.0); // cyan
	vertex_colors[7] = color4(1.0, 1.0, 1.0, 1.0); // white
}
int Index = 0;
void quad(int a, int b, int c, int d)
/*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;

}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(5, 4, 0, 1);
	quad(1, 0, 3, 2);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(3, 0, 4, 7);
	quad(2, 3, 7, 6);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}


void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));


	matrix_loc = glGetUniformLocation(program, "model_view");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/

}
class matrix_stack /*Khai báo lớp stack ma trận*/
{
private:
	static const int MAX = 50;
	mat4 matrices[MAX]; /*Lưu các ma trận của stack*/
	int index; /*Chỉ số truy cập phần tử đầu tiên*/

public: /*Số phần tử tối đa của stack*/
	matrix_stack() { index = 0; } /*Constructor: Khởi gán chỉ số phần tử đầu tiên là 0*/
	void push(const mat4& matrix) {
		matrices[index] = matrix;
		index++;
	};
	mat4 pop() {
		index--;
		return matrices[index];
	};
};
mat4 ctm;
matrix_stack stack;

GLfloat theta[] = { 0,0,0,0 };
void paintColor(GLfloat a, GLfloat b, GLfloat c) {
	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0.0, 1.9, 0.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(mauAnhSang, mauAnhSang, mauAnhSang, 1.0);
	color4 light_specular(0.0, 0.0, 0.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(a / 255.0, b / 255.0, c / 255.0, 1.0);
	color4 material_specular(255.0 / 255.0, 255.0 / 255.0, 255 / 255.0, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}
void veTuong() {
	paintColor(80, 255, 255);
	//Tường sau
	stack.push(ctm);
	ctm *= Translate(0, 0, 7.0)* Scale(9.0, 5.0, 0.05);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();

	//Tường trước
	/*stack.push(ctm);
	ctm *= Translate(0, 0, -2.5)* Scale(5.0, 5.0, 0.05);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();*/

	/*stack.push(ctm);
	ctm *= Translate(1.0, 0, -2.5)* Scale(3.0, 5.0, 0.05);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();

	stack.push(ctm);
	ctm *= Translate(-1.5, 1.5, -2.5)* Scale(2.0, 2.0, 0.05);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();*/


	//Tường trái
	stack.push(ctm);
	ctm *= Translate(-4.5, 0, 0.0)* Scale(0.05, 5.0, 14.0);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();

	//Tường phải
	stack.push(ctm);
	ctm *= Translate(4.5, 0, 0.0)* Scale(0.05, 5.0, 14.0);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();

	//Đáy
	stack.push(ctm);
	ctm *= Translate(0.0, -2.5, 0.0)* Scale(9.0, 0.05, 14.0);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();

	//Trần 
	/*paintColor(236, 236, 236);
	stack.push(ctm);
	ctm *= Translate(0.0, 2.5, 0.0)* Scale(5.0, 0.05, 5.0);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, ctm);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctm = stack.pop();*/



}

GLfloat rotateCuaRaVao;
void veTuongVaCuaRaVao() {
	stack.push(ctm);
	veTuong();
	ctm *= Translate(-2.5, -1.0, -2.5)*RotateY(rotateCuaRaVao);
	//veCuaRaVao();
	ctm = stack.pop();
}
//huan

mat4 ctmHuan;
mat4 Model_BanHuan;
mat4 ctm_banHuan;
void matban(GLfloat w, GLfloat l, GLfloat h)
{
	ctmHuan = Scale(w, h, l);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void chanban(GLfloat w, GLfloat h, GLfloat l)
{
	ctmHuan = Scale(w, h, l);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void KeDuoi(GLfloat w, GLfloat h, GLfloat l)
{
	ctmHuan = Scale(w, h, l);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat gocquay = 0.0;
void cuatu() {
	ctmHuan = Scale(0.3, 0.3, 0.3) * Scale(0.4, 0.2, 0.02);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void tu() {
	paintColor(219, 350, 10);//Màu vàng
	ctmHuan = Scale(0.3, 0.3, 0.5) * Translate(0.0, -0.29, 0.0) * Scale(0.4, 0.02, 0.4);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	ctmHuan = Scale(0.3, 0.3, 0.5) * Translate(0.0, 0.0, 0.2) * Scale(0.4, 0.6, 0.02);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	ctmHuan = Scale(0.3, 0.3, 0.5) * Translate(0.2, 0.0, 0.0) * Scale(0.02, 0.6, 0.4);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/

	ctmHuan = Scale(0.3, 0.3, 0.5) * Translate(-0.2, 0.0, 0.0) * Scale(0.02, 0.6, 0.4);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/\

		ctmHuan = Scale(0.3, 0.3, 0.5) * Translate(0.0, 0.29, 0.0) * Scale(0.4, 0.02, 0.4);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//paintColor(0, 0, 0);//Màu vàng
	////ctm_banHuan = Translate(-0.21f, -0.15f, -0.0f) ;
	//Model_BanHuan *= Scale(0.3, 0.3, 0.3) * Translate(0.2, 0.0, -0.2) * RotateY(gocquay) * Translate(-0.2, 0.0, 0.0);
	//cuatu();
	paintColor(0, 0, 255);//Màu vàng
	//case maytinh
	ctmHuan = Translate(0.0, 0.0, 0.0) * Scale(0.08, 0.1, 0.11);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void manhinh() {
	//tren
	ctmHuan = Translate(0.12, 0.08, -0.01) * Scale(0.2f, 0.01f, 0.01f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//duoi
	ctmHuan = Translate(0.12, 0.2, -0.01) * Scale(0.2f, 0.01f, 0.01f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//trai
	ctmHuan = Translate(0.22, 0.135, -0.01) * Scale(0.01f, 0.12f, 0.01f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//phai
	ctmHuan = Translate(0.02, 0.135, -0.01) * Scale(0.01f, 0.12f, 0.01f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	ctmHuan = Translate(0.12, 0.14, -0.005) * Scale(0.19f, 0.11f, 0.005f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	paintColor(255, 0, 0);
	ctmHuan = Translate(0.12, 0.14, -0.01) * Scale(0.19f, 0.11f, 0.005f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat quayman = 0.0;
GLfloat dcman = 0.0;
void lap() {
	paintColor(0, 0, 255);
	//de
	
	Model_BanHuan *= Translate(dcman, 0.0, 0.0);
	ctmHuan = Translate(0.12, 0.01, 0.0) * Scale(0.1f, 0.01f, 0.05f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//ke
	ctmHuan = Translate(0.12, 0.05, 0.01) * RotateX(-10) * Scale(0.05f, 0.08f, 0.01f);
	glUniformMatrix4fv(matrix_loc, 1, GL_TRUE, Model_BanHuan * ctm_banHuan * ctmHuan);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//man hinh
	//ctm_banHuan = Translate(0.0,0.0,0.1);
	ctm_banHuan = Translate(0.0, 0.10, 0.0) * RotateX(quayman) * Translate(0.0, -0.10, 0.0);
	manhinh();

}


void ban()
{
	paintColor(250, 200, 35);//Màu vàng
	matban(0.6f, 0.4f, 0.01f);
	//giua
	ctm_banHuan = Translate(0.0f, -0.3f, 0.0f);
	chanban(0.02f, 0.02f, 0.3f);
	ctm_banHuan = Translate(0.0f, -0.15f, 0.0f);
	chanban(0.02f, 0.3f, 0.1);
	//trai
	ctm_banHuan = Translate(0.27f, -0.3f, 0.0f);
	chanban(0.02f, 0.02f, 0.3f);
	ctm_banHuan = Translate(0.27f, -0.15f, 0.0f);
	chanban(0.02f, 0.3f, 0.1);
	//phai
	ctm_banHuan = Translate(-0.27f, -0.3f, 0.0f);
	chanban(0.02f, 0.02f, 0.3f);
	ctm_banHuan = Translate(-0.27f, -0.15f, 0.0f);
	chanban(0.02f, 0.3f, 0.1);
	//ke duoi
	paintColor(0, 0, 0);//Màu vàng
	ctm_banHuan = Translate(-0.0f, -0.05f, -0.14f);
	KeDuoi(0.6f, 0.01f, 0.2f);

	//tu 1
	ctm_banHuan = Translate(0.06f, -0.15f, 0.04f);
	tu();
	paintColor(0, 0, 0);//Màu đen
	ctm_banHuan *= Translate(-0.06, 0.0, -0.1) * RotateY(gocquay) * Translate(0.06, 0.0, -0.00);
	cuatu();

	//tu 2
	ctm_banHuan = Translate(-0.19f, -0.15f, 0.04f);
	tu();
	paintColor(0, 0, 0);//Màu đen
	ctm_banHuan *= Translate(-0.06, 0.0, -0.1) * RotateY(gocquay) * Translate(0.06, 0.0, -0.00);
	cuatu();
	ctm_banHuan = identity();
	lap();
	ctm_banHuan = identity();
	Model_BanHuan *= Translate(-0.25, 0.0, 0);
	lap();
}


GLfloat mauDen = 255;

GLfloat n = 0.0, m = 0.0, p = 0.0;
GLfloat anpha = 0.0, beta = 0.0;
GLfloat speed;



GLfloat round1(GLfloat x) {
	return roundf(x * 10) / 10;
}
GLfloat heso = -1.8;
GLfloat dc = -3.5;
void dayban1() {
	
	for (int i = 0; i < 5; i++)
	{
		Model_BanHuan = Translate(3.5, -1.6, dc-heso*i) * RotateY(90) * Scale(3.0, 3.0, 3.0);
		ban();
	}
	
}
void dayban2() {

	for (int i = 0; i < 5; i++)
	{
		Model_BanHuan = Translate(0.4, -1.6, dc - heso * i) * RotateY(270) * Scale(3.0, 3.0, 3.0);
		ban();
	}

}
void dayban3() {

	for (int i = 0; i < 5; i++)
	{
		Model_BanHuan = Translate(-0.5, -1.6, dc - heso * i) * RotateY(90) * Scale(3.0, 3.0, 3.0);
		ban();
	}

}
void dayban4() {

	for (int i = 0; i < 5; i++)
	{
		Model_BanHuan = Translate(-3.5, -1.6, dc - heso * i) * RotateY(270) * Scale(3.0, 3.0, 3.0);
		ban();
	}

}
void dayban() {
	dayban1();
	dayban2();
	dayban3();
	dayban4();
}
//endhuan


GLfloat eyeX = 1.0, eyeZ = -8.0;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stack.push(ctm);
	ctm *= Translate(1.0, -1.5, -1.5)*RotateY(-180);
	//quat();
	ctm = stack.pop();

	veTuongVaCuaRaVao();
	
	dayban();
	
	vec4 eye(eyeX, 4.0, eyeZ, 1);
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 1);
	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-2.5, 2.5, -2.5, 2.5, 1, 17);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glutSwapBuffers();
}
void resharp(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
}

//GLfloat round1(GLfloat x) {
//	return roundf(x * 10) / 10;
//}
bool batden = true;
void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case 'a':
		gocquay += 5.5;
		if (gocquay >= 180)gocquay -= 5.5;
		glutPostRedisplay();

		break;
	case 'b':
		dcman += 0.01;
		//if (gocquay >= 180)gocquay -= 5.5;
		glutPostRedisplay();

		break;
	case 'z':
		quayman += 5.0;
		if (quayman >= 35) quayman -= 5.0;
		glutPostRedisplay();
		break;
		//Di chuyển quạt sang trái
	//case 'q':
	//	translateQuatX += 0.02;
	//	if (translateQuatX + 0.02 >= 2.0) {
	//		translateQuatX = 2.0;
	//	}
	//	glutPostRedisplay();
	//	break;
	//	//Di chuyển quạt sang phải
	//case 'Q':
	//	translateQuatX -= 0.02;
	//	if (translateQuatX - 0.02 <= -2.0) {
	//		translateQuatX = -2.0;
	//	}
	//	glutPostRedisplay();
	//	break;
	//	//Di chuyển quạt lên trước
	//case 'w':
	//	translateQuatZ += 0.02;
	//	if (translateQuatZ + 0.02 >= 2.0) {
	//		translateQuatZ = 2.0;
	//	}
	//	glutPostRedisplay();
	//	break;
	//	//Di chuyển quạt ra sau
	//case 'W':
	//	translateQuatZ -= 0.02;
	//	if (translateQuatZ - 0.02 <= -2.0) {
	//		translateQuatZ = -2.0;
	//	}
	//	glutPostRedisplay();
	//	break;
	//	// Quay đế quạt
	//case 'e':
	//	rotateQuatY += 5;
	//	if (rotateQuatY > 360) {
	//		rotateQuatY -= 360;
	//	}
	//	glutPostRedisplay();
	//	break;
	//	// Quay đế quạt ngược lại
	//case 'E':
	//	rotateQuatY -= 5;
	//	if (rotateQuatY > 360) {
	//		rotateQuatY -= 360;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 'r':
	//	//Hướng đầu quạt lên trên
	//	rotateDauQuatX += 5;
	//	if (rotateDauQuatX + 5 >= 70) {
	//		rotateDauQuatX = 70;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 'R':
	//	//Hướng đầu quạt xuống dưới
	//	rotateDauQuatX -= 5;
	//	if (rotateDauQuatX - 5 <= -10) {
	//		rotateDauQuatX = -10;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 't':
	//	//Hướng đầu quạt sang trái
	//	rotateDauQuatY += 5;
	//	if (rotateDauQuatY + 5 >= 75) {
	//		rotateDauQuatY = 75;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 'T':
	//	//Hướng đầu quạt sang phải
	//	rotateDauQuatY -= 5;
	//	if (rotateDauQuatY - 5 <= -75) {
	//		rotateDauQuatY = -75;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 'u':
	//	//Rút quạt cao lên
	//	translateCayQuatY += 0.02;
	//	if (translateCayQuatY + 0.02 >= 0.4) {
	//		translateCayQuatY = 0.4;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case 'U':
	//	//Hạ quạt thấp xuống
	//	translateCayQuatY -= 0.02;
	//	if (translateCayQuatY - 0.02 <= 0) {
	//		translateCayQuatY = 0;
	//	}
	//	glutPostRedisplay();
	//	break;
	//case '3':
	//	speed = 15;
	//	//Bật quạt số 1
	//	translateNum[0] = 0.0;
	//	translateNum[2] = 0.0;
	//	translateNum[3] = 0.0;
	//	translateNum[1] = -0.025;
	//	glutPostRedisplay();
	//	break;
	//case '2':
	//	//Bật quạt số 2
	//	speed = 10;
	//	translateNum[0] = 0.0;
	//	translateNum[1] = 0.0;
	//	translateNum[3] = 0.0;
	//	translateNum[2] = -0.025;
	//	glutPostRedisplay();
	//	break;
	//case '1':
	//	speed = 5;
	//	//Bật quạt số 3
	//	translateNum[0] = 0.0;
	//	translateNum[1] = 0.0;
	//	translateNum[2] = 0.0;
	//	translateNum[3] = -0.025;
	//	glutPostRedisplay();
	//	break;
	//case '0':
	//	//Tắt quạt
	//	speed = 0;
	//	translateNum[1] = 0.0;
	//	translateNum[2] = 0.0;
	//	translateNum[3] = 0.0;
	//	translateNum[0] = -0.015;
	//	glutPostRedisplay();

	//	break;
	//	//Bật/Tắt đèn
	//case 'o':
	//	if (batden) {
	//		mauDen = 0;
	//		mauAnhSang = 0.4;
	//	}
	//	else {
	//		mauDen = 255;
	//		mauAnhSang = 1.0;
	//	}
	//	batden = !batden;
	//	glutPostRedisplay();
	//	break;

	//case 'a':
	//	if (n + 0.05 <= 2.5) n += 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'A':
	//	if (n - 0.05 > -2.5) n -= 0.05;
	//	glutPostRedisplay();
	//	break;

	//case 's':
	//	if (p + 0.05 <= 1.5)p += 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'S':
	//	if (p - 0.05 > -4.0)p -= 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'd':
	//	anpha += 5;
	//	if (anpha >= 360) anpha -= 360;
	//	glutPostRedisplay();
	//	break;
	//case 'D':
	//	anpha -= 5;
	//	if (anpha <= -360) anpha += 360;
	//	glutPostRedisplay();
	//	break;
	//case 'f':
	//	if (beta < 90) beta += 5;;
	//	glutPostRedisplay();
	//	break;
	//case 'F':
	//	if (beta > 0) beta -= 5;;
	//	glutPostRedisplay();
	//	break;

	//case 'z': translateTuX -= 0.02;
	//	if (translateTuX - 0.02 <= -0.5) translateTuX = -0.5;
	//	glutPostRedisplay();
	//	break;
	//case 'Z': translateTuX += 0.02;
	//	if (translateTuX + 0.02 >= 0.0) translateTuX = 0.0;
	//	glutPostRedisplay();
	//	break;
	//case 'x': translateTuZ -= 0.02;
	//	if (translateTuZ - 0.02 <= -1.0) translateTuZ = -1.0;
	//	glutPostRedisplay();
	//	break;
	//case 'X': translateTuZ += 0.02;
	//	if (translateTuZ + 0.02 >= 0) translateTuZ = 0;
	//	glutPostRedisplay();
	//	break;
	//case 'c':
	//	if (translateTuZ <= -0.2) {
	//		// Tủ đứng sát tường thì không được phép quay
	//		rotateTuY += 5;
	//		if (rotateTuY + 5 >= 360) rotateTuY -= 360;
	//		glutPostRedisplay();
	//	}
	//	break;
	//case 'C':
	//	if (translateTuZ <= -0.2) {
	//		rotateTuY -= 5;
	//		if (rotateTuY - 5 <= 360) rotateTuY += 360;
	//		glutPostRedisplay();
	//	}
	//	break;
	//	//Mở trái tủ
	//case 'v': rotateCanhTu[0] += 5;
	//	if (rotateCanhTu[0] + 5 >= 180) rotateCanhTu[0] = 180;
	//	glutPostRedisplay();
	//	break;
	//	//Đóng trái tủ
	//case 'V': rotateCanhTu[0] -= 5;
	//	if (rotateCanhTu[0] - 5 <= 0)rotateCanhTu[0] = 0;
	//	glutPostRedisplay();
	//	break;
	//case 'b': rotateCanhTu[1] -= 5;
	//	if (rotateCanhTu[1] - 5 <= -180) rotateCanhTu[1] = -180;
	//	glutPostRedisplay();
	//	break;
	//case 'B': rotateCanhTu[1] += 5;
	//	if (rotateCanhTu[1] + 5 >= 0)rotateCanhTu[1] = 0;
	//	glutPostRedisplay();
	//	break;
	//case 'n': translateNganKeo -= 0.02;
	//	if (translateNganKeo - 0.02 < -0.4) translateNganKeo = -0.4;
	//	glutPostRedisplay();
	//	break;
	//case 'N': translateNganKeo += 0.02;
	//	if (translateNganKeo + 0.02 > 0) translateNganKeo = 0;
	//	glutPostRedisplay();
	//	break;

	//case 'g':	//Di chuyển bàn theo trục X
	//	if (translateBanX + 0.05 <= 0.2) translateBanX += 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'G':
	//	if (translateBanX - 0.05 >= -0.8)translateBanX -= 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'h':	//Di chuyển bàn theo trục Z
	//	if (translateBanZ + 0.05 <= 1.8) translateBanZ += 0.05;
	//	glutPostRedisplay();
	//	break;
	//case 'H':
	//	if (translateBanZ - 0.02 >= 0) translateBanZ -= 0.02;
	//	glutPostRedisplay();
	//	break;
	//case 'j':	//Quay bàn
	//	rotateBanY += 5;
	//	if (rotateBanY >= 360) rotateBanY -= 360;
	//	glutPostRedisplay();
	//	break;
	//case 'J':
	//	rotateBanY -= 5;
	//	if (rotateBanY <= -360) rotateBanY += 360;
	//	glutPostRedisplay();
	//	break;
	//case 'k':	//pull
	//	if (translateNganKeoBanZ + 0.01 <= 0.2) translateNganKeoBanZ += 0.01;
	//	glutPostRedisplay();
	//	break;
	//case 'K':	// Push
	//	if (translateNganKeoBanZ - 0.01 >= 0.0) translateNganKeoBanZ -= 0.01;
	//	glutPostRedisplay();
	//	break;

	//	//Di chuyển laptop quanh trục X
	//case 'p':
	//	if (translateLaptopX + 0.01 <= 0.35) translateLaptopX += 0.01;
	//	glutPostRedisplay();
	//	break;
	//case 'P':
	//	if (translateLaptopX - 0.01 >= -0.25) translateLaptopX -= 0.01;
	//	glutPostRedisplay();
	//	break;
	//case 'm':
	//	if (translateLaptopZ + 0.01 <= 0.0) translateLaptopZ += 0.01;
	//	glutPostRedisplay();
	//	break;
	//case 'M':
	//	if (translateLaptopZ - 0.01 >= -0.29) translateLaptopZ -= 0.01;
	//	glutPostRedisplay();
	//	break;
	//	//Gập laptop
	//case 'l': rotateMatTrenLaptop += 5;
	//	if (rotateMatTrenLaptop + 5 >= 90) rotateMatTrenLaptop = 90;
	//	glutPostRedisplay();
	//	break;
	//	//Mở laptop
	//case 'L': rotateMatTrenLaptop -= 5;
	//	if (rotateMatTrenLaptop - 5 <= -90)rotateMatTrenLaptop = -90;
	//	glutPostRedisplay();

	//case 'Y': rotateCuaRaVao += 5;
	//	if (rotateCuaRaVao + 5 >= 0) rotateCuaRaVao = 0;
	//	glutPostRedisplay();
	//	break;
	//	//Mở laptop
	//case 'y': rotateCuaRaVao -= 5;
	//	if (rotateCuaRaVao - 5 <= -80) rotateCuaRaVao = -80;
	//	glutPostRedisplay();
	//	break;


	//	//Thay đổi góc nhìn quanh trục Y
	case '5':
		eyeX = round1(eyeX);
		eyeZ = round1(eyeZ);
		if (eyeX <= 0 && eyeX > -4 && round1(eyeZ) >= -4 && eyeZ < 0) {
			eyeX -= 0.1;
			eyeZ += 0.1;
		}
		else if (eyeX >= -4 && eyeX < 0 && eyeZ >= 0 && eyeZ < 4) {
			eyeX += 0.1;
			eyeZ += 0.1;
		}
		else if (eyeX >= 0 && eyeX < 4 && eyeZ <= 4 && eyeZ > 0) {
			eyeX += 0.1;
			eyeZ -= 0.1;
		}
		else if (eyeX <= 4 && eyeX > 0 && eyeZ <= 0 && eyeZ > -4) {
			eyeX -= 0.1;
			eyeZ -= 0.1;
		}
		glutPostRedisplay();
		break;
		//Mở laptop
	//Thay đổi góc nhìn quanh trục Y ngược chiều kim đồng hồ
	case '4':
		eyeX = round1(eyeX);
		eyeZ = round1(eyeZ);
		if (eyeX >= 0 && eyeX < 4 && round1(eyeZ) >= -4 && eyeZ < 0) {
			eyeX += 0.1;
			eyeZ += 0.1;
		}
		else if (eyeX <= 4 && eyeX > 0 && eyeZ >= 0 && eyeZ < 4) {
			eyeX -= 0.1;
			eyeZ += 0.1;
		}
		else if (eyeX <= 0 && eyeX > -4 && eyeZ <= 4 && eyeZ > 0) {
			eyeX -= 0.1;
			eyeZ -= 0.1;
		}
		else if (eyeX >= -4 && eyeX < 0 && eyeZ <= 0 && eyeZ > -4) {
			eyeX += 0.1;
			eyeZ -= 0.1;
		}
		glutPostRedisplay();
		break;

	}


}


int main(int argc, char **argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Drawing a Cube");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resharp);

	glutMainLoop();
	return 0;
}
