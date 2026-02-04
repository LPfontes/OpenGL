/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  cube.c
 *  This program demonstrates a single modeling transformation,
 *  glScalef() and a single viewing transformation, gluLookAt().
 *  A wireframe cube is rendered.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.0, 0.0, 1.0);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */
   //glTranslatef(0.0, 0.0,-5.0);  // glTranslatef define a posição do objeto
   gluLookAt(0.0, 0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // gluLookAt define a posição da câmera 
   //gluLookAt(3.0, 0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0);
   //gluLookAt(0.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
   GLfloat matrix[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
   for(int i=0; i<4; i++) {
      printf("%f %f %f %f\n", matrix[i], matrix[i+4], matrix[i+8], matrix[i+12]);
   }
   // são matematicamente equivalentes pois a distacia da camera ao objeto é a mesma

   glScalef (1.0, 2.0, 1.0);      /* modeling transformation */ 
   glutWireCube (1.0);
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   //gluPerspective(60.0, 1.0, 1.5, 20.0);
   //gluPerspective(120.0, 1.0, 1.5, 20.0);
   //gluPerspective(60.0, 2.0, 1.5, 20.0);
   //glOrtho(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0); // elimina a sensação de profundidade
   glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
