///Program to view a 2D Object
#include "common.h"

using namespace std;


///Opening window Xposition
int windowPosX2=640;
///Opening window Yposition
int windowPosY2=480;
///Opening window Width
int windowWidth2= 50;
///Opening WIndow Height
int windowHeight2=50;

///Maximum number of vertices in the 2D drwaing
int SIZE = 100;
/// Refresh period in milliseconds
int refreshMillis = 30;
/// Total number of points in 3D model 
int number_of_points;
///Number of edges in the view 
int Edges; 
/// Vertex Matrix of form(index,x,y)
float vertex_matrix[100][3];
/// Edges between which two points is present in the model (Edge Matrix)  
int edges[4950][2]; 
///Binary matrix 1 if there is an edge between those two points
float edges_bin[100][100];
/// 2D coordinates for that drawing
float final_vertices[100][2]; 
/// Final edges between the points (index wise)
int final_edges[100][100];    
///Float to define the rotation about perpendicular axis
float rotX2=0;
 
 void drawtext4(const char* text,int length, int x,int y)
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
void draw2(){
   ///Drawing Function for 2D figures
   glLineWidth(5.0f);
   glColor3f(0.7f, 0.7f, 0.7f);

   for(int i=0; i<number_of_points; i++){
      for(int j=0; j<number_of_points; j++){
         if(edges_bin[i][j] == 1){
            glBegin(GL_LINES);
               glVertex3f(final_vertices[i][0], final_vertices[i][1], 0.0f);
               glVertex3f(final_vertices[j][0], final_vertices[j][1], 0.0f);
            glEnd();
         }
      }
   }

   std::string text4;

   text4= "ROTATION:ARROW KEYS    EXIT:ESCAPE    MINIMIZE:F1   ZOOM:PGUP/PGDN";
   drawtext4(text4.data(),text4.size(),20,25);
}


void display2() {
   /// Callback handler for window re-paint event

   /// Clear color and depth buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   /// To operate on model-view matrix
   glMatrixMode(GL_MODELVIEW);     
   /// Reset the model-view matrix
   glLoadIdentity();                 
   ///Rotate about perpendicular axis
   glRotatef(rotX2, 0.0f, 0.0f, 1.0f);  

   ///Calling drawing function
   draw2();
   /// Swap front and back buffers (of double buffered mode)
   glutSwapBuffers();  
}



void specialKeys2(int key, int x, int y) {
   ///For changing rotation with up/down and right/left keys.
   bool fullScreenMode = true;
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX2   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY2   = glutGet(GLUT_WINDOW_Y);
            windowWidth2  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight2 = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth2, windowHeight2); // Switch into windowed mode
            glutPositionWindow(windowPosX2, windowPosX2);   // Position top-left corner
         }
      break;
      
      case GLUT_KEY_UP:
         rotX2 -= 5.0;
      break;
      
      case GLUT_KEY_DOWN:
         rotX2 += 5.0;
      break;

      case GLUT_KEY_PAGE_UP: // Zoom IN
         for(int i=0; i<number_of_points; i++){
            final_vertices[i][0] *= 1.05f;
            final_vertices[i][1] *= 1.05f;
         }
      break;
      
      case GLUT_KEY_PAGE_DOWN: //Zoom out
         for(int i=0; i<number_of_points; i++){
            final_vertices[i][0] *= 0.95f;
            final_vertices[i][1] *= 0.95f;
         }
      break;
   }     
}


void edge_transformations(){  
   ///Conversion of edge Matrix to Binary edge matrix
   for(int i=0; i<number_of_points; i++)
      for(int j=0; j<number_of_points; j++)
         edges_bin[i][j] = 0;

   for(int i=0; i<number_of_points; i++)
      edges_bin[i][i] =1;

   for(int i=0; i<Edges; i++){
      edges_bin[edges[i][0]][edges[i][1]]=1;
      edges_bin[edges[i][1]][edges[i][0]]=1;
   }
}


void get_coordinates(){
   ///Getting 2D vertices from vertex Matrix in order
   int index;
   for(int i=0; i<number_of_points; i++){
      index = (int)vertex_matrix[i][0];

      final_vertices[index][0] = vertex_matrix[i][1];  
      final_vertices[index][1] = vertex_matrix[i][2];  
   }

}


void call_2D_view(int argc, char** argv) {
   /// Main function: GLUT runs as a console application starting at main() 

   ifstream infile;
   infile.open(*argv);
   infile >> number_of_points;
 
   for(int i=0; i<number_of_points; i++){
      infile >> vertex_matrix[i][0] >> vertex_matrix[i][1] >> vertex_matrix[i][2];  //(index,x,y)
   }
   
   infile >> Edges; //Number of neighbours in top view
   for(int i=0; i<Edges; i++){
      infile >> edges[i][0] >> edges[i][1]; //Put the indices of the vertices that are neighbours
   }
   infile.close();

   char title[] = "2D_View";  // Windowed mode's title
   ///Self Made Functions
   get_coordinates();  //Converts the 2D coordinates to 3D coordinates
   edge_transformations();  //Get three n*n matrices of edges

   ///Glut Functions
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowPosX2 ,windowPosY2 );  // Initial window width and height
   glutInitWindowPosition(windowWidth2,windowHeight2); // Initial window top-left corner (x, y)
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display2);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
   glutSpecialFunc(specialKeys2); // Register callback handler for special-key event
   glutFullScreen();             // Put into full screen
   initGL();                       // Our own OpenGL initialization
   glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   glutMainLoop();                 // Enter the infinite event-processing loop
   return;
}
