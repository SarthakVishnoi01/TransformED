#include "common.h"
using namespace std;
/* Global variables */
char title[] = "3D Shapes to 2D shapes (no hidden edge)";

//Specifies the angle of rotation about X axis     
GLfloat rotX3=0.0f; 
//Specifies the angle of rotation about X axis
GLfloat rotY3=1.0f; 
///Number of faces in 3D model
int number_of_faces1 = 0;
///Number of vertices in each face
int face_index1[30];  // Number of vertices in each face
///X coordinates of points (face and which vertex of that face)
float Xcor1[100][30]; // Maximum number of points in each face is 100
///Y coordinates of points (face and which vertex of that face)
float Ycor1[100][30];
///Z coordinates of points (face and which vertex of that face)
float Zcor1[100][30];
///Number of edges in 3D model
int number_of_edges1=0; // maximum will be 100

///Opening window Xposition
int windowPosX3=640;
///Opening window Yposition
int windowPosY3=480;
///Opening window Width
int windowWidth3= 50;
///Opening Window Height
int windowHeight3=50;

///X coordinate of first vertex of edge
float XcorA1[100]; //X coordinate of first vertex of the edge
///X coordinate of second vertex of edge
float XcorB1[100]; // PointA and PointB form an edge
///Y coordinate of first vertex of edge
float YcorA1[100];
///Y coordinate of second vertex of edge
float YcorB1[100];
///Z coordinate of first vertex of edge
float ZcorA1[100];
///Z coordinate of second vertex of edge
float ZcorB1[100];


void drawtext1(const char* text,int length, int x,int y)
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

void draw3(){
   ///Draw user input 3D figure
   glColor3f(0.7f, 0.7f, 0.7f); // Solid Color is Grey
   for(int i=0; i<number_of_faces1; i++){
      glBegin(GL_POLYGON);
         for(int j=0; j<face_index1[i];j++){
            glVertex3f(Xcor1[i][j], Ycor1[i][j], Zcor1[i][j]);
         }
      glEnd();   
   }
   
   glColor3f(0.0f, 0.0f, 0.0f); //EdgeColor =Black
   
   glBegin(GL_LINES);
      for(int i=0; i<number_of_edges1; i++){
         glVertex3f(XcorA1[i],YcorA1[i],ZcorA1[i]);
         glVertex3f(XcorB1[i],YcorB1[i],ZcorB1[i]);
      }
   glEnd();

   std::string text;

   text= "ROTATION:ARROW KEYS    EXIT:ESCAPE    MINIMIZE:F1   ZOOM:PGUP/PGDN";
   drawtext1(text.data(),text.size(),20,25);
   
}


 
void display3() {
   /// Handler for window-repaint event. Called back when the window first appears and whenever the window needs to be re-painted. 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
   
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix

   //Draw rotatable isometric view
   glTranslatef(0.0f, 0.0f, 0.0f);  // Move right and into the screen
   glRotatef(rotX3, 1.0f, 0.0f, 0.0f);  // Rotate about x-axis [NEW]
   glRotatef(rotY3, 0.0f, 1.0f, 0.0f);  //Rotate about y axis
   draw3();
   
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
void specialKeys3(int key, int x, int y) {
	///Rotation of object via arrow keys, toggle between full screen mode and window mode as well as zoom in/zoom out the object
   bool fullScreenMode = true;
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX3   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY3   = glutGet(GLUT_WINDOW_Y);
            windowWidth3 = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight3 = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth3, windowHeight3); // Switch into windowed mode
            glutPositionWindow(windowPosX3, windowPosX3);   // Position top-left corner
         }
      break;
      
      case GLUT_KEY_UP:
         rotX3 -= 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_DOWN:
         rotX3+= 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_LEFT:
         rotY3 -= 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_RIGHT:
         rotY3 += 10.0;
   //glutPostRedisplay();
      break;
      
      case GLUT_KEY_PAGE_UP: // Zoom IN
         for(int i=0; i<number_of_faces1; i++){
            for(int j=0; j< face_index1[i]; j++){
               Xcor1[i][j] *= 1.05f;
               Ycor1[i][j] *= 1.05f;
               Zcor1[i][j] *= 1.05f;
            }
         }
         for(int i=0; i<number_of_edges1; i++){
            XcorA1[i] *= 1.05f;
            YcorA1[i] *= 1.05f;
            ZcorA1[i] *= 1.05f;
            XcorB1[i] *= 1.05f;
            YcorB1[i] *= 1.05f;
            ZcorB1[i] *= 1.05f;
         }
      break;
      
      case GLUT_KEY_PAGE_DOWN: //Zoom out
         for(int i=0; i<number_of_faces1; i++){
            for(int j=0; j< face_index1[i]; j++){
               Xcor1[i][j] *= 0.95f;
               Ycor1[i][j] *= 0.95f;
               Zcor1[i][j] *= 0.95f;
            }
         }
         for(int i=0; i<number_of_edges1; i++){
           XcorA1[i] *= 0.95f;
           YcorA1[i] *= 0.95f;
           ZcorA1[i] *= 0.95f;
           XcorB1[i] *= 0.95f;
           YcorB1[i] *= 0.95f;
           ZcorB1[i] *= 0.95f;
         }
      break;
   }   
}

 

void call_3D_view(int argc, char** argv) {
/// Main function: GLUT runs as a console application starting at main() 
   ifstream infile;
   infile.open(*argv);
   //cout<<argv[1];
   if(!infile){
      cerr << "Cannot open file";
      exit(1);
   }
   
   infile>> number_of_faces1; //Number of faces = 6
   for(int i=0;i<number_of_faces1;i++){
      infile >> face_index1[i]; // each will be equal to 4
      for(int j=0; j<face_index1[i]; j++){
         infile >> Xcor1[i][j] >> Ycor1[i][j] >> Zcor1[i][j];  //input the coordinates
      }
   }

   /*Edge Input*/
   
   infile >> number_of_edges1;
   for(int i=0; i<number_of_edges1; i++){
      infile >> XcorA1[i] >> YcorA1[i] >> ZcorA1[i]; 
      infile >> XcorB1[i] >> YcorB1[i] >> ZcorB1[i];
   }

   /*Glut functions*/
   
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth3, windowHeight3);  // Initial window width and height
   glutInitWindowPosition(windowPosX3, windowPosY3); // Initial window top-left corner (x, y)
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display3);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
   glutSpecialFunc(specialKeys3); // Register callback handler for special-key event
   glutFullScreen();             // Put into full screen
   initGL();                       // Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   glutMainLoop();                 // Enter the infinite event-processing loop
   
   return ;
}
