//
//  ArUco-OpenGL.cpp
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//


#include "ArUco-OpenGL.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\calib3d.hpp>

// Liste des id des marqueurs qu'on utilise
const vector<int> listMarkerId = { 85, 90, 144, 161, 166, 214, 227, 244 };

// Constructor
ArUco::ArUco(string intrinFileName, float markerSize) {
   // Initializing attributes
   m_IntrinsicFile= intrinFileName;
   m_MarkerSize   = markerSize;
   // read camera parameters if passed
   m_CameraParams.readFromXMLFile(intrinFileName);
  
}

// Destructor
ArUco::~ArUco() {}

void ArUco::resizeCameraParams(cv::Size newSize) {
	m_CameraParams.resize(newSize);
}

// Detect marker and draw things
void ArUco::doWork(Mat inputImg) {
   m_InputImage   = inputImg;
   m_GlWindowSize = m_InputImage.size();
   m_CameraParams.resize(m_InputImage.size());
   resize(m_GlWindowSize.width, m_GlWindowSize.height);
}

// Draw axis function
void ArUco::drawAxis(float axisSize) {
   // X
   glColor3f (1,0,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(axisSize,0.0f, 0.0f); // ending point of the line
   glEnd( );
   
   // Y
   glColor3f (0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, axisSize, 0.0f); // ending point of the line
   glEnd( );
   
   // Z
   glColor3f (0,0,1);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, 0.0f, axisSize); // ending point of the line
   glEnd( );
}

void ArUco::drawWireCube(GLdouble size) {
    drawBox(size, GL_LINE_LOOP);
}

// Fonction qui dessine un cube de différentes manières (type)
void ArUco::drawBox(GLfloat size, GLenum type)
{
		static const GLfloat n[6][3] =
		{
		  {-1.0, 0.0, 0.0},
		  {0.0, 1.0, 0.0},
		  {1.0, 0.0, 0.0},
		  {0.0, -1.0, 0.0},
		  {0.0, 0.0, 1.0},
		  {0.0, 0.0, -1.0}
		};
		static const GLint faces[6][4] =
		{
		  {0, 1, 2, 3},
		  {3, 2, 6, 7},
		  {7, 6, 5, 4},
		  {4, 5, 1, 0},
		  {5, 6, 2, 1},
		  {7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		for (i = 5; i >= 0; i--) {
			glBegin(type);
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
}


void ArUco::drawPyramid(GLfloat size) {
    
	static const GLfloat n[4][3] =
	{
	  {0.0f, 1.0f, 1.0f},  // Normale pour la première face
	{0.0f, 1.0f, -1.0f}, // Normale pour la deuxième face
	{1.0f, 1.0f, 0.0f},  // Normale pour la troisième face
	{-1.0f, 1.0f, 0.0f}  // Normale pour la quatrième face
	};
	static const GLint faces[4][3] =
	{
	  {0, 1, 2},
	  {0, 2, 3},
	  {0, 3, 4},
	  {0, 4, 1},
	};
	GLfloat v[5][3];
	GLint i;

	v[0][0] = v[0][1] = 0.0f;
	v[0][2] = size/2;

	v[1][0] = v[1][1] = -size/2;
	v[1][2] = -size / 2;

	v[2][0] = size/2;
	v[2][1] = -size/2;
	v[2][2] = -size / 2;

	v[3][0] = v[3][1] = size/2;
	v[3][2] = -size / 2;

	v[4][1] = size/2;
	v[4][0] = -size / 2;
	v[4][2] = -size / 2;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 0.0f, 0.0f);

	for (i = 0; i <= 3; i++) {
		glBegin(GL_TRIANGLES);
		glNormal3fv(&n[i][0]);
		glVertex3fv(&v[faces[i][0]][0]);
		glVertex3fv(&v[faces[i][1]][0]);
		glVertex3fv(&v[faces[i][2]][0]);
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (i = 0; i <= 3; i++) {
		glBegin(GL_TRIANGLES);
		glNormal3fv(&n[i][0]);
		glVertex3fv(&v[faces[i][0]][0]);
		glVertex3fv(&v[faces[i][1]][0]);
		glVertex3fv(&v[faces[i][2]][0]);
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ArUco::drawCube(GLfloat size) {

	GLfloat v[8][3] = {
		{-size/2,-size / 2, -size / 2},
		{ size / 2, -size / 2, -size / 2},
		{ size / 2,  size / 2, -size / 2},
		{-size / 2,  size / 2, -size / 2},
		{-size / 2, -size / 2,  size / 2},
		{ size / 2, -size / 2,  size / 2},
		{ size / 2,  size / 2,  size / 2},
		{-size / 2,  size / 2,  size / 2}
	};

	GLint faces[5][4] = {
	//{0, 1, 2, 3}, 
	{4, 5, 6, 7}, 
	{0, 1, 5, 4}, 
	{2, 3, 7, 6}, 
	{1, 2, 6, 5}, 
	{4, 7, 3, 0}  
	};

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	for (int i = 0; i < 5; i++) {
		glVertex3fv(&v[faces[i][0]][0]); // Premier sommet de la face
		glVertex3fv(&v[faces[i][1]][0]); // Deuxième sommet de la face
		glVertex3fv(&v[faces[i][2]][0]); // Troisième sommet de la face
		glVertex3fv(&v[faces[i][3]][0]); // Quatrième sommet de la face
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.0f, .0f);
	glBegin(GL_QUADS);
	for (int i = 0; i < 5; i++) {
		glVertex3fv(&v[faces[i][0]][0]); // Premier sommet de la face
		glVertex3fv(&v[faces[i][1]][0]); // Deuxième sommet de la face
		glVertex3fv(&v[faces[i][2]][0]); // Troisième sommet de la face
		glVertex3fv(&v[faces[i][3]][0]); // Quatrième sommet de la face
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void ArUco::drawCylinder(GLfloat size) {

	float radius = size / 2;
	float height = size;
	int slices = 20;
	float angleStep = 2.0f * M_PI / slices; // L'angle entre chaque segment de la base

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.0f, 0.0f);

	// Dessiner la base inférieure
	glBegin(GL_POLYGON);
	for (int i = 0; i < slices; i++) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, -size / 2);
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0f, 1.0f, 0.0f);

	// Dessiner la base inférieure
	glBegin(GL_POLYGON);
	for (int i = 0; i < slices; i++) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, -size/2);  
	}
	glEnd();

	// Dessiner la base supérieure
	glBegin(GL_POLYGON);
	for (int i = 0; i < slices; i++) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, size/2);  // Base à z = height
	}
	glEnd();

	// Dessiner les faces latérales
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < slices; i++) {
		float angle = i * angleStep;
		float nextAngle = (i + 1) * angleStep;

		// Points de la base inférieure
		float x0 = radius * cos(angle);
		float y0 = radius * sin(angle);
		// Points de la base supérieure
		float x1 = radius * cos(nextAngle);
		float y1 = radius * sin(nextAngle);

		// Points pour les faces latérales
		glVertex3f(x0, y0, -size / 2);       
		glVertex3f(x0, y0, size/2);     
		glVertex3f(x1, y1, -size / 2);       
		glVertex3f(x1, y1, size / 2);     
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.0f, 0.0f);

	// Dessiner la base supérieure
	glBegin(GL_POLYGON);
	for (int i = 0; i < slices; i++) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, size / 2);  // Base à z = height
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// GLUT functionnalities

// Drawing function
void ArUco::drawScene() {
   // If we do not have an image we don't do anyhting
   if (m_ResizedImage.rows==0)
      return;
   
   // On "reset" les matrices OpenGL de ModelView et de Projection
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // On deinit une vue orthographique de la taille de l'image OpenGL
   glOrtho(0, m_GlWindowSize.width, 0, m_GlWindowSize.height, -1.0, 1.0);
   // on definit le viewport correspond a un rendu "plein ecran"
   glViewport(0, 0, m_GlWindowSize.width , m_GlWindowSize.height);

   // on desactive les textures
   glDisable(GL_TEXTURE_2D);

   // On "flippe" l'axe des Y car OpenCV et OpenGL on un axe Y inverse pour les images/textures
   glPixelZoom( 1, -1);

   // On definit la position ou l'on va ecrire dans l'image
   glRasterPos3f(0, m_GlWindowSize.height, -1.0f);
   
   // On "dessine" les pixels contenus dans l'image OpenCV m_ResizedImage (donc l'image de la Webcam qui nous sert de fond)
   glDrawPixels (m_GlWindowSize.width, m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_ResizedImage.ptr(0));
   
   // On active ensuite le depth test pour les objets 3D
   glEnable(GL_DEPTH_TEST);
   
   // On passe en mode projection pour definir la bonne projection calculee par ArUco
   glMatrixMode(GL_PROJECTION);
   double proj_matrix[16];
   m_CameraParams.glGetProjectionMatrix(m_ResizedImage.size(),m_GlWindowSize,proj_matrix,0.01,100);
   glLoadIdentity();
   // on charge la matrice d'ArUco 
   glLoadMatrixd(proj_matrix);
   
   // On affiche le nombre de marqueurs (ne sert a rien)
   double modelview_matrix[16];
   std::cout << "Number of markers: " << m_Markers.size() << std::endl;
   
   // On desactive le depth test
   glDisable(GL_DEPTH_TEST);

   // Pour chaque marqueur detecte
   for (unsigned int m=0;m<m_Markers.size();m++)
   {
	   cout << m_Markers[m].id << endl;
	   // On recupere la matrice de modelview qui correspond au marqueur [m]
       m_Markers[m].glGetModelViewMatrix(modelview_matrix);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
	   // on charge cette matrice pour se placer dans le repere de ce marqueur [m] 
       glLoadMatrixd(modelview_matrix);
      
      // On dessine les axes X Y Z
      //drawAxis(m_MarkerSize);

	   // On se deplace sur Z de la moitie du marqueur pour dessiner "sur" le plan du marqueur
	   glTranslatef(0, 0, m_MarkerSize / 2.);

	   // On sauvegarde la matrice courante
	   glPushMatrix();

	   if (m_Markers[m].id == 85 || m_Markers[m].id == 90) {
		   drawPyramid(m_MarkerSize);
	  }

	   if (m_Markers[m].id == 144 || m_Markers[m].id == 161) {
		   drawCube(m_MarkerSize);
	   }

	   if (m_Markers[m].id == 166 || m_Markers[m].id == 214) {
		   drawCylinder(m_MarkerSize);
	   }

	  


	  // on choisit une couleur
	  //glColor3f(1, 0.4f, 0.4f);
	  //On dessine une cube en fil de fer
      //drawWireCube( m_MarkerSize );

	  // On sauvegarde la matrice courante
	  glPushMatrix();

      // On re=charge la matrice que l'on a sauvegarde
      glPopMatrix();
   }
   
   // Desactivation du depth test
   glDisable(GL_DEPTH_TEST);
}



// Idle function
void ArUco::idle(Mat newImage) {
   // Getting new image
   m_InputImage = newImage.clone();
  
   // Undistort image based on distorsion parameters
   m_UndInputImage.create(m_InputImage.size(),CV_8UC3);
   
   //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
   cv::cvtColor(m_InputImage,m_InputImage,cv::COLOR_BGR2RGB);
   
   //remove distorion in image ==> does not work very well (the YML file is not that of my camera)
   //cv::undistort(m_InputImage,m_UndInputImage, m_CameraParams.CameraMatrix, m_CameraParams.Distorsion);
   m_UndInputImage = m_InputImage.clone();
   
   //resize the image to the size of the GL window
   cv::resize(m_UndInputImage,m_ResizedImage,m_GlWindowSize);

   //detect markers
   m_PPDetector.detect(m_ResizedImage, m_Markers, m_CameraParams, m_MarkerSize, false);

}

// Resize function
void ArUco::resize(GLsizei iWidth, GLsizei iHeight) {
   m_GlWindowSize=Size(iWidth,iHeight);
   
   //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
   if (iWidth*3%4!=0) {
      iWidth+=iWidth*3%4;//resize to avoid padding
      resize(iWidth, m_GlWindowSize.height);
   }
   else {
      //resize the image to the size of the GL window
      if (m_UndInputImage.rows!=0)
         cv::resize(m_UndInputImage, m_ResizedImage, m_GlWindowSize);
   }

}

// Test using ArUco to display a 3D cube in OpenCV
void ArUco::draw3DCube(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dCube(img, m_Markers[markerInd], m_CameraParams); 
   }
}

void ArUco::draw3DAxis(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dAxis(img, m_Markers[markerInd], m_CameraParams); 
   }
   
}
