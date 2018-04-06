#include "common.h"

using namespace std;
/// refresh interval in milliseconds [NEW]
int refreshMills = 100;  
///Specifies the angle of rotation about X axis     
GLfloat rotX1=0.0f;
///Specifies the angle of rotation about Y axis
GLfloat rotY1=1.0f; 
///Number of faces in 3D figure
int number_of_faces = 0;
///Number of vertices in each face 
int face_index[30];
///X coordinates of points (face and which vertex of that face)
float Xcor[100][30]; // Maximum number of points in each face is 100
///Y coordinates of points (face and which vertex of that face)
float Ycor[100][30];
///Z coordinates of points (face and which vertex of that face)
float Zcor[100][30];
///Number of edges in 3D model
int number_of_edges=0; 
///equation of planes for all faces: ax+by+cz+d=0
float planes[4][30]; 
///Opening window Xposition
int windowPosX=640;
///Opening window Yposition
int windowPosY=480;
///Opening window Width
int windowWidth= 50;
///Opening Window Height
int windowHeight=50;

///X coordinate of first vertex of edge
float XcorA[100]; //X coordinate of first vertex of the edge
///X coordinate of second vertex of edge
float XcorB[100]; // PointA and PointB form an edge
///Y coordinate of first vertex of edge
float YcorA[100];
///Y coordinate of second vertex of edge
float YcorB[100];
///Z coordinate of first vertex of edge
float ZcorA[100];
///Z coordinate of second vertex of edge
float ZcorB[100];

/// front[FACES][EDGES] check z coefficient (whether it is hidden from a particular face in front view then false)
bool front[30][100];
/// top[FACES][EDGES] check y coefficient (whether it is hidden from a particular face in top view then false)
bool top[30][100];
/// side[FACES][EDGES] check x coefficient (whether it is hidden from a particular face in side view then false)
bool side[30][100];

///AND of all faces for a particular edge in front view
bool finalFront[100]; //And of all faces
///AND of all faces for a particular edge in top view
bool finalTop[100];
///AND of all faces for a particular edge in side view
bool finalSide[100];

void drawtext3(const char* text,int length, int x,int y)
{
	///Function to include text in GL Window
   glMatrixMode(GL_PROJECTION);
   double *matrix = new double[16];
   glGetDoublev(GL_PROJECTION_MATRIX,matrix);
   glLoadIdentity();
   glOrtho(0,800,0,600  ,-5,5);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPushMatrix();
   glLoadIdentity();
   glRasterPos2i(x,y);
   for(int i=0;i<length;i++)
   {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)text[i]);
   }
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(matrix);
   glMatrixMode(GL_MODELVIEW);
}


//int window(0);
// Full-screen or windowed mode?

/* Initialize OpenGL Graphics */
void initGL() {
	///Function to initialize OpenGL Graphics
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
   //First three are eye, next three are 
   //where they are seeing, next three are which is up on the screen
   gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, -1000.0, 0.0, 1.0, 0.0);
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/*I have got the equations of all the planes*/
void get_planes(){
	///Function which computes equation of each plane (each face)
   float x1,x2,y1,y2,z1,z2,p,q,r;
   for(int i=0; i<number_of_faces; i++){ //take first three coordinates XCor[0][i]
      x1=Xcor[0][i]-Xcor[1][i];
      x2=Xcor[1][i]-Xcor[2][i];
      y1=Ycor[0][i]-Ycor[1][i];
      y2=Ycor[1][i]-Ycor[2][i];
      z1=Zcor[0][i]-Zcor[1][i];
      z2=Zcor[1][i]-Zcor[2][i];
      p=y1*z2-y2*z1;
      q=-1.0f*(x1*z2-x2*z1);
      r=x1*y2-x2*y1;
      planes[0][i] = p;
      planes[1][i] = q;
      planes[2][i] = r;
      planes[3][i] = -1*(p*Xcor[0][i] + q*Ycor[0][i] + r*Zcor[0][i]);
   }
}

void visible(){ 
	///Function to check visibility of each edge wrt each face (plane)
   for(int i=0; i<number_of_faces; i++){
      for(int j=0; j<number_of_edges; j++){
         if(planes[2][i]*(planes[0][i]*XcorA[j] + planes[1][i]*YcorA[j] + planes[2][i]*ZcorA[j] + planes[3][i]) <0 || planes[2][i]*(planes[0][i]*XcorB[j] + planes[1][i]*YcorB[j] + planes[2][i]*ZcorB[j] + planes[3][i]) <0)
            front[i][j]=false;
         else
            front[i][j]=true;
         if(planes[1][i]*(planes[0][i]*XcorA[j] + planes[1][i]*YcorA[j] + planes[2][i]*ZcorA[j] + planes[3][i]) <0 || planes[1][i]*(planes[0][i]*XcorB[j] + planes[1][i]*YcorB[j] + planes[2][i]*ZcorB[j] + planes[3][i]) <0)
            top[i][j]=false;
         else
            top[i][j]=true;
         if(planes[0][i]*(planes[0][i]*XcorA[j] + planes[1][i]*YcorA[j] + planes[2][i]*ZcorA[j] + planes[3][i]) <0 || planes[0][i]*(planes[0][i]*XcorB[j] + planes[1][i]*YcorB[j] + planes[2][i]*ZcorB[j] + planes[3][i]) <0)
            side[i][j]=false;
         else
            side[i][j]=true;
      }
   }
}

void final_vis(){
	///Function to check if a particular edge is hidden in that particular view or not
   bool counterf, countert,counters;
   for(int i=0; i<number_of_edges; i++){
      counterf=true;
      countert=true;
      counters=true;
      for(int j=0; j<number_of_faces; j++){
         counterf = counterf && front[j][i];
         countert = countert && top[j][i];
         counters = counters && side[j][i];
      }
      finalFront[i] = counterf;
      finalTop[i] = countert;
      finalSide[i] = counters;
   }
}

void draw3D(){
   ///Draw the 3D figure
   glColor3f(0.7f, 0.7f, 0.7f); // Solid Color is Grey
   for(int i=0; i<number_of_faces; i++){
      glBegin(GL_POLYGON);
         for(int j=0; j<face_index[i];j++){
            glVertex3f(Xcor[i][j], Ycor[i][j], Zcor[i][j]);
         }
      glEnd();   
   }
   
   glColor3f(0.0f, 0.0f, 0.0f); //EdgeColor =Black
   
   glBegin(GL_LINES);
      for(int i=0; i<number_of_edges; i++){
         glVertex3f(XcorA[i],YcorA[i],ZcorA[i]);
         glVertex3f(XcorB[i],YcorB[i],ZcorB[i]);
      }
   glEnd();

   std::string text3;

   text3= "ROTATION:ARROW KEYS    EXIT:ESCAPE    MINIMIZE:F1   ZOOM:PGUP/PGDN";
   drawtext3(text3.data(),text3.size(),20,5);
   
}

void draw2D_Top(){
	///Draw top view (Drop y coordinate)
   for(int i=0; i<number_of_edges; i++){
      if(!finalTop[i]){ //it's hidden line
         glPushAttrib(GL_ENABLE_BIT); 
         glLineStipple(3, 0xAAAA);
         glEnable(GL_LINE_STIPPLE);
         glBegin(GL_LINES);
            glVertex3f(XcorA[i],-1.0*ZcorA[i],0.0f);
            glVertex3f(XcorB[i],-1.0*ZcorB[i],0.0f);
         glEnd();
         glPopAttrib();
      }

      else{//truth is true ie draw solid line
         glBegin(GL_LINES);
            glVertex3f(XcorA[i],-1.0*ZcorA[i],0.0f);
            glVertex3f(XcorB[i],-1.0*ZcorB[i],0.0f);
         glEnd();
      }
   }
}

void draw2D_Front(){
   ///Draw front view (Drop z coordinate)
   for(int i=0; i<number_of_edges; i++){
      if(!finalFront[i]){ //it's hidden line
         glPushAttrib(GL_ENABLE_BIT); 
         glLineStipple(3, 0xAAAA);
         glEnable(GL_LINE_STIPPLE);
         glBegin(GL_LINES);
            glVertex3f(XcorA[i],YcorA[i],0.0f);
            glVertex3f(XcorB[i],YcorB[i],0.0f);
         glEnd();
         glPopAttrib();
      }
      else{//truth is true ie draw solid line
         glBegin(GL_LINES);
            glVertex3f(XcorA[i],YcorA[i],0.0f);
            glVertex3f(XcorB[i],YcorB[i],0.0f);
         glEnd();
      }
   }
} 

void draw2D_Side(){
	///Draw side view (Drop x coordinate)
   for(int i=0; i<number_of_edges; i++){
      if(!finalSide[i]){ //it's hidden line
         glPushAttrib(GL_ENABLE_BIT); 
         glLineStipple(3, 0xAAAA);
         glEnable(GL_LINE_STIPPLE);
         glBegin(GL_LINES);
            glVertex3f(-1.0*ZcorA[i],YcorA[i],0.0f);
            glVertex3f(-1.0*ZcorB[i],YcorB[i],0.0f);
         glEnd();
         glPopAttrib();
      }
      else{//truth is true ie draw solid line
         glBegin(GL_LINES);
            glVertex3f(-1.0*ZcorA[i],YcorA[i],0.0f);
            glVertex3f(-1.0*ZcorB[i],YcorB[i],0.0f);
         glEnd();
      }

   }
}

void display() {
	/// Handler for window-repaint event. Called back when the window first appears and whenever the window needs to be re-painted. 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
   
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix

   /*I'll have to draw four figures in the same page */

   get_planes();
   visible();
   final_vis();
   //draw top view
   glPushMatrix();
   glTranslatef(-3.8f, 1.5f, 0.0f);
   //glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
   draw2D_Top();
   glPopMatrix();
   //glTranslatef(3.5f, -3.0f, 0.0f);
   //glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

   //draw front view
   glPushMatrix();
   glTranslatef(-3.8f, -3.2f, 0.0f);
   draw2D_Front();
   glPopMatrix();
   
   //Draw side view
   glPushMatrix();
   glTranslatef(3.8f, -3.2f, 0.0f);
   draw2D_Side();
   glPopMatrix();

   //Draw rotatable isometric view
   glTranslatef(3.8f, 1.5f, 0.0f);  // Move right and into the screen
   glRotatef(rotX1, 1.0f, 0.0f, 0.0f);  // Rotate about x-axis [NEW]
   glRotatef(rotY1, 0.0f, 1.0f, 0.0f);  //Rotate about y axis
   draw3D();

   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 

void timer(int value) {
	/// Called back when timer expired [NEW] 
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}
void keyboard(unsigned char key, int x, int y) {
	///To exit the window press Esc
   switch (key) {
      case 27:     // ESC key
         glutLeaveMainLoop();
         break;
   }
}            

void reshape(GLsizei width, GLsizei height) {  
	///Handler for window re-size event. Called back when the window first appears and whenever the window is re-sized with its new width and height 
	// GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
 
   // Set up orthographic projection view [NEW]
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, -10.0, 10.0);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     glOrtho(-5.0, 5.0, -5.0 / aspect, 5.0 / aspect, -10, 10);
   }
}

void specialKeys(int key, int x, int y) {
	///Rotation of object via arrow keys, toggle between full screen mode and window mode as well as zoom in/zoom out the object
   bool fullScreenMode = true; 
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
         }
      break;
      
      case GLUT_KEY_UP:
         rotX1 -= 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_DOWN:
         rotX1 += 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_LEFT:
         rotY1 -= 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_RIGHT:
         rotY1 += 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_PAGE_UP: // Zoom IN
         for(int i=0; i<number_of_faces; i++){
            for(int j=0; j< face_index[i]; j++){
               Xcor[i][j] *= 1.05f;
               Ycor[i][j] *= 1.05f;
               Zcor[i][j] *= 1.05f;
            }
         }
         for(int i=0; i<number_of_edges; i++){
            XcorA[i] *= 1.05f;
            YcorA[i] *= 1.05f;
            ZcorA[i] *= 1.05f;
            XcorB[i] *= 1.05f;
            YcorB[i] *= 1.05f;
            ZcorB[i] *= 1.05f;
         }
      break;
      
      case GLUT_KEY_PAGE_DOWN: //Zoom out
         for(int i=0; i<number_of_faces; i++){
            for(int j=0; j< face_index[i]; j++){
               Xcor[i][j] *= 0.95f;
               Ycor[i][j] *= 0.95f;
               Zcor[i][j] *= 0.95f;
            }
         }
         for(int i=0; i<number_of_edges; i++){
           XcorA[i] *= 0.95f;
           YcorA[i] *= 0.95f;
           ZcorA[i] *= 0.95f;
           XcorB[i] *= 0.95f;
           YcorB[i] *= 0.95f;
           ZcorB[i] *= 0.95f;
         }
      break;
   }   
}
 

void call_3D_to_2D(int argc, char** argv) {
	///Main like function: GLUT runs as a console application starting at main() 
   /*Inputs*/
   /*Vertex Input*/
   ifstream infile;
   infile.open(*argv);
   //cout<<argv[1];
   if(!infile){
      cerr << "Cannot open file";
      exit(1);
   }
   
   infile>> number_of_faces; //Number of faces = 6
   for(int i=0;i<number_of_faces;i++){
      infile >> face_index[i]; // each will be equal to 4
      for(int j=0; j<face_index[i]; j++){
         infile >> Xcor[i][j] >> Ycor[i][j] >> Zcor[i][j];  //input the coordinates
      }
   }

   /*Edge Input*/
   
   infile >> number_of_edges;
   for(int i=0; i<number_of_edges; i++){
      infile >> XcorA[i] >> YcorA[i] >> ZcorA[i]; 
      infile >> XcorB[i] >> YcorB[i] >> ZcorB[i];
   }
   
   /*Glut functions*/
   char title[] = "3D Shapes to 2D shapes (with hidden edges hidden edge)";
   
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
   glutSpecialFunc(specialKeys); // Register callback handler for special-key event
   glutFullScreen();             // Put into full screen
   initGL();                       // Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   glutMainLoop();                 // Enter the infinite event-processing loop

   return ;
}
