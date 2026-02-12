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
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */
#include <GL/glut.h>
#include <stdlib.h>
typedef struct {
    float r;
    float g;
    float b;
} Color;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Escala;

typedef struct {
    float x;
    float y;
    float z;
} Posicao;

Color cinza = {0.5f, 0.5f, 0.5f};
Color azul  = {0.0f, 0.0f, 0.8f};
Color verde = {0.0f, 0.8f, 0.0f};
Color amarelo = {0.8f, 0.8f, 0.0f};
Color vermelho = {0.8f, 0.0f, 0.0f};

Escala escalaBase = {2.0f, 0.2f, 2.0f};
Escala escalaOmbro = {2.0f, 0.4f, 1.0f};
Escala escalaCotovelo = {2.0f, 0.41f, 1.0f};
Escala escalaPulso = {0.4f, 0.4f, 0.8f};
Escala escalaDedos = {0.5f, 0.2f, 0.2f};
Escala escalaCaixa = {0.5f, 0.5f, 0.5f};
Escala escalaRecipienteParede1 = {2.0f, 1.0f, 0.2f};
Escala escalaRecipienteParede2 = {0.2f, 1.0f, 2.0f};
Escala escalaRecipienteFundo = {1.9f, 0.1f, 1.9f};

Posicao posicaoCamera = {6.0f, 6.0f, 6.0f};
Posicao alvoCamera = {0.0f, 0.0f, 0.0f};
Posicao posicaoRecipienteParede1 = {0.0f, 0.5f, -1.0f};
Posicao posicaoRecipienteParede2 = {0.0f, 0.5f, 1.0f};
Posicao posicaoRecipienteParede3 = {-1.0f, 0.5f, 0.0f};
Posicao posicaoRecipienteParede4 = {1.0f, 0.5f, 0.0f};
Posicao posicaoRecipiente = {0.0f, -0.2f, 3.0f};
Posicao posicaoCaixa = {4.0f, 0.0f, 0.0f};


static int anguloBase = 0;
static int anguloOmbro = 35;     
static int anguloCotovelo = -35;   
static int torcaoAntebraco = 0;
static int anguloPulso = 0;
static int anguloDedos = 45;
static int segurando = 0;
static GLfloat caixaMatrix[16];

static float tamanhoBase = 1.0;
static float tamanhoOmbro = 1.1;
static float tamanhoCotovelo = 1.0;
static float tamanhoPulso = 1.0;
static float tamanhoDedos = 1.0;


void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable(GL_DEPTH_TEST); 
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(posicaoCaixa.x, posicaoCaixa.y, posicaoCaixa.z);
   glGetFloatv(GL_MODELVIEW_MATRIX, caixaMatrix);
   glPopMatrix();
}

void desenhaTexto(char *string, float x, float y) {
    glRasterPos2f(x, y); // Define a posição onde o texto começa
    while (*string) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string);
        string++;
    }
}

void legenda() {
   glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // Salva a projeção (perspectiva)
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 800); // Cria um sistema 2D fixo (baseado no tamanho da janela)

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0); // Cor branca para o texto
    desenhaTexto("COMANDOS:", 10, 770);
    desenhaTexto("A/D: Base | S/W: Ombro | E/Q: Cotovelo", 10, 750);
    desenhaTexto("R/T: Torcao | Y/U: Pulso | F/G: Dedos | Espaco: Pegar", 10, 730);
    desenhaTexto("ESC: Sair", 10, 710);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Restaura a projeção original
    glMatrixMode(GL_MODELVIEW);
}

// Função auxiliar para desenhar o cubo sólido com a borda aramada
void desenhaCuboComBorda(Color cor, float tamanho ) {
    glColor3f(cor.r, cor.g, cor.b);
    glutSolidCube(tamanho);
    glColor3f(1.0, 1.0, 1.0); // branca
    glutWireCube(tamanho + 0.01); // Borda um pouco maior para destacar      
}

void desenhaBase() {
    glRotatef((GLfloat) anguloBase, 0.0, 1.0, 0.0); // Rotaciona a base em torno do eixo Y
    glPushMatrix();
        glScalef(escalaBase.x, escalaBase.y, escalaBase.z); // Escala a base
        desenhaCuboComBorda(cinza, tamanhoBase);
    glPopMatrix();
}

void desenhaOmbro() {
    glTranslatef(0.0, escalaBase.y / 2.0f, 0.0); // Move para o topo da base 
    glRotatef((GLfloat) anguloOmbro, 0.0, 0.0, 1.0); // Rotaciona o ombro em torno do eixo Z
    glTranslatef(escalaOmbro.x / 2.0f, 0.0, 0.0);  // Move para o centro do braço superior
    glPushMatrix(); 
        glScalef(escalaOmbro.x, escalaOmbro.y, escalaOmbro.z);
        desenhaCuboComBorda(azul, tamanhoOmbro);
    glPopMatrix(); 
}

void desenhaCotovelo() {
    glTranslatef(escalaOmbro.x / 2.0f, 0.0, 0.0); // Move para o final do braço superior
    glRotatef((GLfloat) anguloCotovelo, 0.0, 0.0, 1.0); // Rotaciona o cotovelo em torno do eixo Z
    glRotatef((GLfloat) torcaoAntebraco, 1.0, 0.0, 0.0); // Torção do antebraço em torno do eixo X
    glTranslatef(escalaCotovelo.x / 2.0f, 0.0, 0.0);  // Move para o centro do braço inferior
    glPushMatrix();
        glScalef(escalaCotovelo.x, escalaCotovelo.y, escalaCotovelo.z);
        desenhaCuboComBorda(verde,tamanhoCotovelo);
    glPopMatrix();
}

void desenhaPulso() {
    glTranslatef(escalaCotovelo.x / 2.0f, 0.0, 0.0);  // Move para o final do braço inferior
    glRotatef((GLfloat) anguloPulso, 0.0, 0.0, 1.0); // Rotaciona o pulso em torno do eixo Z
    glTranslatef(escalaPulso.x / 2.0f, 0.0, 0.0);  // Move para o centro do pulso
    glPushMatrix();
        glScalef(escalaPulso.x, escalaPulso.y, escalaPulso.z);
        desenhaCuboComBorda(amarelo, tamanhoPulso);
    glPopMatrix();
}

void desenhaDedos() {
    glTranslatef(escalaPulso.x / 2.0f, 0.0, 0.0);
    
    // Dedo Superior parte 1
    glPushMatrix();
        glTranslatef(-0.2, escalaPulso.y / 2, 0.0); // Move para a posição do dedo superior
        glRotatef((GLfloat) anguloDedos, 0.0, 0.0, 1.0); // Rotaciona os dedos em torno do eixo Z
        
        glPushMatrix();
            glTranslatef(escalaDedos.x / 2.0f, 0.0, 0.0); // Move para o centro do dedo
            glScalef(escalaDedos.x, escalaDedos.y, escalaDedos.z);
            desenhaCuboComBorda(vermelho, tamanhoDedos);
        glPopMatrix();

        // Dedo Superior parte 2
        glTranslatef(escalaDedos.x - 0.1 , 0.06, 0.0);
        glRotatef(-60.0, 0.0, 0.0, 1.0);
        glPushMatrix();
            glTranslatef(escalaDedos.x / 2.0f, 0.0, 0.0);
            glScalef(escalaDedos.x, escalaDedos.y, escalaDedos.z);
            desenhaCuboComBorda(vermelho, tamanhoDedos - 0.05);
        glPopMatrix();
    glPopMatrix();

    // Dedo Inferior parte 1
    glPushMatrix();
        glTranslatef(-0.2, -escalaPulso.y /2, 0.0); // Move para a posição do dedo inferior 
        glRotatef((GLfloat) -anguloDedos, 0.0, 0.0, 1.0); // Rotaciona os dedos em torno do eixo Z
        
        glPushMatrix();
            glTranslatef(escalaDedos.x / 2.0f, 0.0, 0.0); // Move para o centro do dedo
            glScalef(escalaDedos.x, escalaDedos.y, escalaDedos.z);
            desenhaCuboComBorda(vermelho, tamanhoDedos);
        glPopMatrix();
        
        // Dedo Inferior parte 2
        glTranslatef(escalaDedos.x - 0.1, -0.06, 0.0);
        glRotatef(60.0, 0.0, 0.0, 1.0);
        glPushMatrix();
            glTranslatef(escalaDedos.x / 2.0f, 0.0, 0.0);
            glScalef(escalaDedos.x, escalaDedos.y, escalaDedos.z);
            desenhaCuboComBorda(vermelho, tamanhoDedos - 0.05);
        glPopMatrix();
    glPopMatrix();
}

void calculaPosicaoCaixa() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRotatef((GLfloat) anguloBase, 0.0, 1.0, 0.0);
    glTranslatef(0.0, escalaBase.y / 2.0f, 0.0);
    glRotatef((GLfloat) anguloOmbro, 0.0, 0.0, 1.0);
    glTranslatef(escalaOmbro.x / 2.0f, 0.0, 0.0);
    glTranslatef(escalaOmbro.x / 2.0f, 0.0, 0.0);
    glRotatef((GLfloat) anguloCotovelo, 0.0, 0.0, 1.0);
    glRotatef((GLfloat) torcaoAntebraco, 1.0, 0.0, 0.0);
    glTranslatef(escalaCotovelo.x / 2.0f, 0.0, 0.0);
    glTranslatef(escalaCotovelo.x / 2.0f, 0.0, 0.0);
    glRotatef((GLfloat) anguloPulso, 0.0, 0.0, 1.0);
    glTranslatef(escalaPulso.x / 2.0f, 0.0, 0.0);
    glTranslatef(escalaPulso.x / 2.0f, 0.0, 0.0);
    glTranslatef(0.4, 0.0, 0.0);
    
    glGetFloatv(GL_MODELVIEW_MATRIX, caixaMatrix);
    
    caixaMatrix[0] = 1.0f; caixaMatrix[1] = 0.0f; caixaMatrix[2] = 0.0f;
    caixaMatrix[4] = 0.0f; caixaMatrix[5] = 1.0f; caixaMatrix[6] = 0.0f;
    caixaMatrix[8] = 0.0f; caixaMatrix[9] = 0.0f; caixaMatrix[10] = 1.0f;
    caixaMatrix[13] = 0.25f;
    
    glPopMatrix();
}

void desenhaCaixa(){
    if (segurando) return;
    glPushMatrix();
    glMultMatrixf(caixaMatrix);
    glScalef(0.5, 0.5, 0.5);
    desenhaCuboComBorda(vermelho, 1.0);
    glPopMatrix();
}
void desenhaCaixaMao(){
    if (!segurando) return;
    glPushMatrix();
    glTranslatef(0.4, 0.0, 0.0);
    glScalef(0.5, 0.5, 0.5);
    desenhaCuboComBorda(vermelho, 1.0);
    glPopMatrix();
}
void desenhaRecipiente(){
    glPushMatrix();
    glTranslatef(posicaoRecipiente.x, posicaoRecipiente.y, posicaoRecipiente.z); // Posiciona o recipiente no eixo Z
    // Fundo
    glPushMatrix();
    glScalef(escalaRecipienteFundo.x, escalaRecipienteFundo.y, escalaRecipienteFundo.z);
    desenhaCuboComBorda(cinza, 1.0);
    glPopMatrix();

    // Paredes
        glPushMatrix();
        glTranslatef(posicaoRecipienteParede1.x, posicaoRecipienteParede1.y, posicaoRecipienteParede1.z);
        glScalef(escalaRecipienteParede1.x, escalaRecipienteParede1.y, escalaRecipienteParede1.z);
        desenhaCuboComBorda(azul, 1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoRecipienteParede2.x, posicaoRecipienteParede2.y, posicaoRecipienteParede2.z);
        glScalef(escalaRecipienteParede1.x, escalaRecipienteParede1.y, escalaRecipienteParede1.z);
        desenhaCuboComBorda(azul, 1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoRecipienteParede3.x, posicaoRecipienteParede3.y, posicaoRecipienteParede3.z);
        glScalef(escalaRecipienteParede2.x, escalaRecipienteParede2.y, escalaRecipienteParede2.z);
        desenhaCuboComBorda(azul, 1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoRecipienteParede4.x, posicaoRecipienteParede4.y, posicaoRecipienteParede4.z);
    glScalef(escalaRecipienteParede2.x, escalaRecipienteParede2.y, escalaRecipienteParede2.z);
    desenhaCuboComBorda(azul, 1.0);
    glPopMatrix();

    glPopMatrix();
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    
    gluLookAt(posicaoCamera.x, posicaoCamera.y, posicaoCamera.z, alvoCamera.x, alvoCamera.y, alvoCamera.z, 0.0, 1.0, 0.0); // Camera na diagonal superior
    desenhaRecipiente();
    desenhaCaixa();
    desenhaBase();
    desenhaOmbro();
    desenhaCotovelo();
    desenhaPulso();
    desenhaDedos();
    desenhaCaixaMao();
    legenda();
    glutSwapBuffers();  // Troca os buffers para exibir a cena renderizada
}
void keyboard(unsigned char key, int x, int y) {
   int limit_abertura = 60;
   int limit_fechamento = 20;
   switch (key) {
        case 'a': anguloBase = (anguloBase + 5) % 360; break;
        case 'd': anguloBase = (anguloBase - 5) % 360; break;
        case 's': anguloOmbro = (anguloOmbro + 5) % 360; break;
        case 'w': anguloOmbro = (anguloOmbro - 5) % 360; break;
        case 'e': anguloCotovelo = (anguloCotovelo + 5) % 360; break;
        case 'q': anguloCotovelo = (anguloCotovelo - 5) % 360; break;
        case 'r': torcaoAntebraco = (torcaoAntebraco + 5) % 360; break;
        case 't': torcaoAntebraco = (torcaoAntebraco - 5) % 360; break;
        case 'y': anguloPulso = (anguloPulso + 5) % 360; break;
        case 'u': anguloPulso = (anguloPulso - 5) % 360; break;
        case 'f': if (anguloDedos < limit_abertura) anguloDedos += 5; break;
        case 'g': if (anguloDedos > limit_fechamento) anguloDedos -= 5; break;
        case 'P':
            anguloBase = 0; anguloOmbro = 0; anguloCotovelo = 0;
            torcaoAntebraco = 0; anguloPulso = 0; anguloDedos = 0;
            break;
        case ' ': 
            if (segurando) {
                calculaPosicaoCaixa();
                segurando = 0;
            } else {
                segurando = 1;
            }
            break;
        case 27: exit(0); break;
   }
   glutPostRedisplay();
}
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

int main(int argc, char** argv)
{
    int larguraJanela = 800;
    int alturaJanela = 800;

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // 1. Obtém a resolução do monitor (Screen)
    int larguraTela = glutGet(GLUT_SCREEN_WIDTH);
    int alturaTela = glutGet(GLUT_SCREEN_HEIGHT);

    int posX = (larguraTela - larguraJanela) / 2;
    int posY = (alturaTela - alturaJanela) / 2;

    glutInitWindowSize (larguraJanela, alturaJanela); 
    glutInitWindowPosition (posX, posY); // Define a posição calculada
    
    glutCreateWindow (argv[0]);
    
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
