//
//  Window.cpp
//  CSE167 Final Project
//
//  Created by Ruiqing Qiu on 12/7/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Window.h"
#include <stdlib.h>

#include <math.h>
#define PI 3.14159265f

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
Matrix4 Window::world = Matrix4();
Ball Window::ball = Ball(Vector3(0,1,0),2);
vector<Brick> Window::b_list;
vector<Brick> Window::rope_list;
vector<btRigidBody*> Window::bodies;
int Movement;
Vector3 lastPoint = Vector3(0, 0, 0);
Vector3 lastPoint_z = Vector3(0, 0, 0);

Cursor Window::cursor = Cursor(Vector3(0,1,0),2);
LeapListener Window::listener;
//for moving the camera
double eye_x = 0;
double eye_y = 15;
double eye_z = 30;


//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    cout << w << ", " << h << endl;
    cerr << "Window::reshapeCallback called" << endl;
    width = w;
    height = h;
    glViewport(0, 0, w, h);  // set new viewport size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(Globals::homework_num == 2){
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0);// set perspective projection viewing frustum
    glTranslatef(0, 0, Globals::camera->position.z);// move camera back 20 units so that it looks at the origin (or else it's in the origin)
    }
    else if(Globals::homework_num == 1){
        if (Window::width > Window::height)
        {
            //			glFrustum (-aspect, aspect, -1.0, 1.0, 1.0, 10000.0);
            glFrustum (-double(width)/(double)height * 1, double(width)/(double)height * 1, -1, 1, 1, 1000);
        } else
        {
        //			glFrustum (-1.0, 1.0, -aspect, aspect, 1.0, 10000.0);
            glFrustum (-double(width)/(double)height * 1, double(width)/(double)height * 1, -1, 1, 1, 1000);
        }
    }
    glMatrixMode(GL_MODELVIEW);
    
}
// During translation, we also have to maintain the GL_TEXTURE8, used in the shadow shader
// to determine if a vertex is in the shadow.
void startTranslate(float x,float y,float z)
{
    glPushMatrix();
    glTranslatef(x,y,z);
    
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    glPushMatrix();
    glTranslatef(x,y,z);
}

void endTranslate()
{
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}



void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (Window::width > Window::height)
    {
        //			glFrustum (-aspect, aspect, -1.0, 1.0, 1.0, 10000.0);
        glFrustum (-double(Window::width)/(double)Window::height * 1, double(Window::width)/(double)Window::height * 1, -1, 1, 1, 1000);
    } else
    {
        //			glFrustum (-1.0, 1.0, -aspect, aspect, 1.0, 10000.0);
        glFrustum (-double(Window::width)/(double)Window::height * 1, double(Window::width)/(double)Window::height * 1, -1, 1, 1, 1000);
    }
    //gluPerspective(60,Window::width/Window::height,1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);

//    glLoadIdentity();
//    Globals::camera->e->x = position_x;
//    Globals::camera->e->y = position_y;
//    Globals::camera->e->z = position_z;
//    Globals::camera->d->x = lookAt_x;
//    Globals::camera->d->y = lookAt_y;
//    Globals::camera->d->z = lookAt_z;
//    Globals::camera->update();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    Matrix4 glmatrix = Globals::camera->getMatrix() * Window::world;
//    glmatrix.transpose();
//    glLoadMatrixd(glmatrix.getPointer());
    gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);

}
void setTextureMatrix(void)
{
    static double modelView[16];
    static double projection[16];
    
    // This is matrix transform every coordinate x,y,z
    // x = x* 0.5 + 0.5
    // y = y* 0.5 + 0.5
    // z = z* 0.5 + 0.5
    // Moving from unit cube [-1,1] to [0,1]
    const GLdouble bias[16] = {
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0};
    
    // Grab modelview and transformation matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    
    glLoadIdentity();
    glLoadMatrixd(bias);
    
    // concatating all matrice into one.
    glMultMatrixd (projection);
    glMultMatrixd (modelView);
    
    // Go back to normal matrix mode
    glMatrixMode(GL_MODELVIEW);
}
//This draw function is for homework 1
void Window::draw(){
    glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
    // Tell OpenGL what ModelView matrix to use:
    //gluLookAt(0, 15, 30, 0, 0, 0, 0, 1, 0);
    //gluLookAt(eye_x,eye_y,eye_z,0, 0, 0,0, 1, 0);
    Matrix4 glmatrix;
    glmatrix.identity();
    glmatrix = Globals::camera->getMatrix() * world;
    glmatrix.transpose();
    //glPushMatrix();
    //glLoadIdentity();
    glLoadMatrixd(glmatrix.getPointer());
    glDisable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    glColor3f(0.7, 1, 1);
    glNormal3f(0, 1, 0);
    glVertex3f(-1000, -1, -1000);
    glVertex3f(1000, -1, -1000);
    glVertex3f(1000, -1, 1000);
    glVertex3f(-1000, -1, 1000);
    glEnd();
    //glPopMatrix();
    glEnable(GL_CULL_FACE);
    
    for(int i = 0; i < bodies.size();i++){
        if (bodies[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
            glColor3f(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX);
            renderSphere(bodies[i]);
        }
        
        if (bodies[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE) {
            renderPlane(bodies[i]);
            cout << "render plane" << endl;
        }
        
        if (bodies[i]->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE) {
            
            renderCube(bodies[i]);
            
        }
    }
    ball.draw(Globals::camera->getMatrix()*glmatrix*world, ball.radius);
    //ball.draw(Globals::camera->getMatrix()*glmatrix*world, ball.radius);
    for(int i = 0; i < b_list.size(); i++){
        b_list[i].draw(Globals::camera->getMatrix()*glmatrix*world, 2.0);
    }
    //    for(int i = 0; i < rope_list.size();i++){
    //        rope_list[i].draw(Globals::camera->getMatrix()*glmatrix*world, 0.4);
    //    }
    //Globals::dynamicsWorld->debugDrawWorld();
    //Globals::softworld->debugDrawWorld();
    glFlush();
}

void Window::draw2(){
    
    glColor4f(0.3f,0.3f,0.3f,1);
    glBegin(GL_QUADS);
    glVertex3f(-50,0,-50);
    glVertex3f(-50,0, 50);
    glVertex3f( 50,0, 50);
    glVertex3f( 50,0,-50);
    glEnd();
    glColor4f(0.9f,0.9f,0.9f,1);
    startTranslate(-10, 20, 0);
    glColor3f(listener.color.x,listener.color.y,listener.color.z);
    glutSolidCube(2);
    endTranslate();
    cursor.setLocation(listener.pos.x, listener.pos.y, listener.pos.z);
    cursor.physics(listener.pos.x, listener.pos.y, listener.pos.z);
    cursor.draw(Globals::camera->getMatrix()*world, cursor.radius);
    glLineWidth(2.5);
    if(listener.sample_points.size() > 1){
    for(int i = 0; i < listener.sample_points.size()-1;i++){
        glColor3f(listener.corresponding_colors[i].x,listener.corresponding_colors[i].y,listener.corresponding_colors[i].z);
        glBegin(GL_LINES);
        glVertex3f(listener.sample_points[i].x,listener.sample_points[i].y,listener.sample_points[i].z);
        glVertex3f(listener.sample_points[i+1].x,listener.sample_points[i+1].y,listener.sample_points[i+1].z);
        glEnd();
    }
    }

//    for(int i = 0; i < listener.blist.size(); i++){
//        listener.blist[i].draw(Globals::camera->getMatrix()*glmatrix*world, 2.0);
//    }

}
void Window::draw3(){
}
void Window::displayCallback()
{
    clock_t startTime = clock();
    //cout << listener.draw_mode << endl;
    Globals::dynamicsWorld->stepSimulation(1 / 60.f, 10);
    //Globals::softworld->stepSimulation(1.0f/60.f,0);
    //tmp.print_height();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
    if(Globals::homework_num == 1){
        draw();
    }
    else if(Globals::homework_num == 2){
       

        //draw2();
        //First step: Render from the light POV to a FBO, story depth values only
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,Globals::fboId);	//Rendering offscreen
        
        //Using the fixed pipeline to render to the depthbuffer
        glUseProgramObjectARB(0);
        
        // In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
        glViewport(0,0,Window::width * 4,Window::height* 4);
        
        // Clear previous frame values
        glClear(GL_DEPTH_BUFFER_BIT);
        
        //Disable color rendering, we only want to write to the Z-Buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
        setupMatrices(Globals::light_pos.x,Globals::light_pos.y,Globals::light_pos.z,0,0,-5);
        
        // Culling switching, rendering only backface, this is done to avoid self-shadowing
        glCullFace(GL_FRONT);
        draw2();
        
        //Save modelview/projection matrice into texture7, also add a biais
        setTextureMatrix();
        
        
        // Now rendering from the camera POV, using the FBO to generate shadows
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
        
        glViewport(0,0,Window::width,Window::height);
        
        //Enabling color write (previously disabled for light POV z-buffer rendering)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        // Clear previous frame values
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Using the shadow shader
        glUseProgramObjectARB(Globals::shadowShaderId);
        glUniform1iARB(Globals::shadowMapUniform,7);
        glActiveTextureARB(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D,Globals::depthTextureId);
        
        setupMatrices(Globals::camera->e->x,Globals::camera->e->y,Globals::camera->e->z,
                      Globals::camera->d->x,Globals::camera->d->y,Globals::camera->d->z);
        
        glCullFace(GL_BACK);
        draw2();
        
        // DEBUG only. this piece of code draw the depth buffer onscreen
        
//        glUseProgramObjectARB(0);
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        glOrtho(-Window::width/2,Window::width/2,-Window::height/2,Window::height/2,1,20);
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        glColor4f(1,1,1,1);
//        glActiveTextureARB(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D,Globals::depthTextureId);
//        glEnable(GL_TEXTURE_2D);
//        glTranslated(0,0,-1);
//        glBegin(GL_QUADS);
//        glTexCoord2d(0,0);glVertex3f(0,0,0);
//        glTexCoord2d(1,0);glVertex3f(Window::width/2,0,0);
//        glTexCoord2d(1,1);glVertex3f(Window::width/2,Window::height/2,0);
//        glTexCoord2d(0,1);glVertex3f(0,Window::height/2,0);
        
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    


    //draw();
    
    
    glutSwapBuffers();
    
    clock_t endTime = clock();
    //cout << "frame rate: " << 1.0/(float((endTime - startTime))/CLOCKS_PER_SEC) << endl;
}
void Window::idleCallback()
{
    //Globals::cube.spin(1.0);   // rotate cube; if it spins too fast try smaller values and vice versa
    displayCallback();
}

void Window::processSpecialKeys(int key, int x, int y){
    //Display ball mode
    if(key == GLUT_KEY_F1){
    }
}
/**
 * Method for process keyboard actions
 **/
void Window::processNormalKeys(unsigned char key, int x, int y){
    if (key == 27){
        exit(0);
    }
    else if(key == ' '){
        //physics_cleanup();
        physics_setup();
        initWalls();
        Globals::camera->e->x = 0;
        Globals::camera->e->y = 15;
        Globals::camera->e->z = 30;
        Globals::camera->update();
        world.identity();
    }
    //move the camera y up
    else if(key == 'w'){
        Globals::camera->e->y+=10;
    }
    //move the camer y down
    else if(key == 's'){
        Globals::camera->e->y-=10;
    }
    //move the camera x up
    else if(key == 'd'){
        Globals::camera->e->x+=10;
    }
    //move the camera x down
    else if(key == 'a'){
        Globals::camera->e->x-=10;
    }
    else if(key == 'q'){
        Globals::camera->e->z-=10;
    }
    else if(key == 'e'){
        Globals::camera->e->z+=10;
    }
    else if (key == 'h'){
        Window::ball.rb->applyCentralImpulse(btVector3(0,0,100));
    }
    Globals::camera->update();
}

Vector3 Window::trackBallMapping(int x, int y){
    Vector3 v = Vector3(0,0,0);
    float d;
    v.x = (2.0*x - width) / width;
    v.y = (height - 2.0*y) / height;
    v.z = 0.0;
    d = v.length();
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    v.normalize(); // Still need to normalize, since we only capped d, not v.
    return v;
}



int gPickingConstraintId = 0;
btVector3 gOldPickingPos;
btVector3 gHitPos(-1,-1,-1);
float gOldPickingDist  = 0.f;
btRigidBody* pickedBody = 0;//for deactivation state
//
btVector3 getRayTo(int x,int y)
{
    
    float top = 1.f;
    float bottom = -1.f;
    float nearPlane = 1.f;
    float tanFov = (top-bottom)*0.5f / nearPlane;
    float fov = btScalar(2.0) * btAtan(tanFov);
    
    btVector3	rayFrom = btVector3(Globals::camera->e->x,Globals::camera->e->y,Globals::camera->e->z);
    Vector3 tmp = *Globals::camera->d - *Globals::camera->e;
    btVector3 rayForward = btVector3(tmp.getX(), tmp.getY(), tmp.getZ());
    rayForward.normalize();
    float farPlane = 1000.f;
    rayForward*= farPlane;
    
    btVector3 rightOffset;
    btVector3 vertical = btVector3(0,1,0);
    
    btVector3 hor;
    hor = rayForward.cross(vertical);
    hor.normalize();
    vertical = hor.cross(rayForward);
    vertical.normalize();
    
    float tanfov = tanf(0.5f*fov);
    
    
    hor *= 2.f * farPlane * tanfov;
    vertical *= 2.f * farPlane * tanfov;
    
    btScalar aspect;
    
    if (Window::width > Window::height)
    {
        aspect = Window::width / (btScalar)Window::height;
        
        hor*=aspect;
    } else
    {
        aspect = Window::height / (btScalar)Window::width;
        vertical*=aspect;
    }
    
    btVector3 rayToCenter = rayFrom + rayForward;
    btVector3 dHor = hor * 1.f/float(Window::width);
    btVector3 dVert = vertical * 1.f/float(Window::height);
    
    
    btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
    rayTo += btScalar(x) * dHor;
    rayTo -= btScalar(y) * dVert;
    return rayTo;
}

btScalar mousePickClamping = 30.f;

int m_mouseButtons;
int	m_mouseOldX;
int	m_mouseOldY;
btTypedConstraint*		m_pickConstraint;

float force = 0.0;
void	Window::mouseMove(int x,int y)
{
        Vector3 direction = Vector3(0, 0, 0);
        float pixel_diff;
        float rot_angle, zoom_factor;
        Vector3 curPoint = Vector3(0,0,0);
        lastPoint.print("last point is ");
        switch (Movement) {
            case 0:{
                cout << "case 0" << endl;
                curPoint = trackBallMapping(x, y);//Map the mouse position to a logical sphere location
                direction = curPoint - lastPoint;
                float velocity = direction.length();
                if(velocity > 0.0001){
                    Vector3 rotAxis = lastPoint.cross(lastPoint, curPoint);
                    rot_angle = velocity * 100;
                    cout << "rot angle " << rot_angle << endl;
                    GLfloat objectXform[16];
                    glGetFloatv(GL_MODELVIEW_MATRIX, objectXform);
                    glLoadIdentity();
                    glRotatef(rot_angle, rotAxis.x, rotAxis.y, rotAxis.z);
                    glMultMatrixf(objectXform);
                    Matrix4 tmp = Matrix4();
                    tmp.makeRotate(rot_angle, rotAxis);
                    //Globals::bunny.getMatrix() = Globals::bunny.getMatrix() * tmp;
                    Vector4 new_e = Vector4(Globals::camera->e->getX(), Globals::camera->e->getY(), Globals::camera->e->getZ(), 0);
                    tmp.transpose();
                    new_e = tmp * new_e;
                    Globals::camera->e->x = new_e.get_x();
                    Globals::camera->e->y = new_e.get_y();
                    Globals::camera->e->z = new_e.get_z();
                    Globals::camera->update();
                    //world = tmp * world;
                }
                break;
            }
            case 1:
                //cout << "case 1, right mouse button" << endl;
                pixel_diff =y - lastPoint_z.y;
                //cout << "pixel diff " << pixel_diff << endl;
                force = 100*pixel_diff;
                break;
//            /* Zoom on mouse right button */
//    //        case 1:
//    //            cout << "case 1" << endl;
//    //            pixel_diff = y - lastPoint_z.y;
//    //            cout << "pixel diff " << pixel_diff << endl;
//    //            zoom_factor = 1.0 + pixel_diff/50;
//    //            cout << "zoom factor " << zoom_factor << endl;
//    //            Matrix4 tmp = Matrix4();
//    //            tmp.makeScale(zoom_factor, zoom_factor, zoom_factor);
//    //            world = world * tmp;
//    //            break;
        }
        lastPoint = curPoint;
        lastPoint_z = Vector3(x, y, 0);
    if (m_pickConstraint)
    {
        //move the constraint pivot
        
        if (m_pickConstraint->getConstraintType() == D6_CONSTRAINT_TYPE)
        {
            btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pickConstraint);
            if (pickCon)
            {
                //keep it at the same picking distance
                
                btVector3 newRayTo = getRayTo(x,y);
                btVector3 rayFrom;
                btVector3 oldPivotInB = pickCon->getFrameOffsetA().getOrigin();
                
                btVector3 newPivotB;
                
                    rayFrom = btVector3(Globals::camera->e->x,Globals::camera->e->y,Globals::camera->e->z);;
                    btVector3 dir = newRayTo-rayFrom;
                    dir.normalize();
                    dir *= gOldPickingDist;
                    
                    newPivotB = rayFrom + dir;
                
                pickCon->getFrameOffsetA().setOrigin(newPivotB);
            }
            
        } else
        {
            btPoint2PointConstraint* pickCon = static_cast<btPoint2PointConstraint*>(m_pickConstraint);
            if (pickCon)
            {
                //keep it at the same picking distance
                
                btVector3 newRayTo = getRayTo(x,y);
                btVector3 rayFrom;
                btVector3 oldPivotInB = pickCon->getPivotInB();
                btVector3 newPivotB;
                
                    rayFrom = btVector3(Globals::camera->e->x,Globals::camera->e->y,Globals::camera->e->z);;
                    btVector3 dir = newRayTo-rayFrom;
                    dir.normalize();
                    dir *= gOldPickingDist;
                    
                    newPivotB = rayFrom + dir;
                pickCon->setPivotB(newPivotB);
            }
        }
    }
    
    float dx, dy;
    dx = btScalar(x) - m_mouseOldX;
    dy = btScalar(y) - m_mouseOldY;
    
    m_mouseOldX = x;
    m_mouseOldY = y;
}

void Window::mouse(int button, int state, int x, int y)
{
    if (state == 0)
    {
        m_mouseButtons |= 1<<button;
    } else
    {
        m_mouseButtons = 0;
    }
        
    m_mouseOldX = x;
    m_mouseOldY = y;
        
    //printf("button %i, state %i, x=%i,y=%i\n",button,state,x,y);
    //button 0, state 0 means left mouse down
    switch(button){
            //Left-mouse button is being held down
        case GLUT_RIGHT_BUTTON:
            Movement = 0;
            break;
        case GLUT_LEFT_BUTTON:
            Movement = 1;
            if(state == 0){
            //Vector3 m_rayTo = GetPickRay(x, y);
            btVector3 m_rayTo = getRayTo(x, y);
            cout << m_rayTo.getX() << " " << m_rayTo.getY() << " " << m_rayTo.getZ() << endl;
            //btVector3 btRayTo = btVector3(m_rayTo.x, m_rayTo.y, m_rayTo.z);
            
            //btVector3 End = btRayTo;
            btVector3 End = m_rayTo;
            btVector3 Start = btVector3(Globals::camera->e->x,Globals::camera->e->y,Globals::camera->e->z) ;
            btVector3 Normal;
            btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
            
            Globals::dynamicsWorld->rayTest(Start, End, RayCallback);
            // Perform raycast
            
            if(RayCallback.hasHit()) {
                End = RayCallback.m_hitPointWorld;
                Normal = RayCallback.m_hitNormalWorld;
                // Do some clever stuff here
                cout << "hit" << endl;
                btRigidBody* body = const_cast<btRigidBody*>(btRigidBody::upcast(RayCallback.m_collisionObject));
                if (body){
                    //other exclusions?
                    if (!(body->isStaticObject() || body->isKinematicObject()))
                    {
                        pickedBody = body;
                        pickedBody->setActivationState(DISABLE_DEACTIVATION);
                        
                        btVector3 pickPos = RayCallback.m_hitPointWorld;
                        printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
                        
                        btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
                        
                        btTransform tr;
                        tr.setIdentity();
                        tr.setOrigin(localPivot);
                        btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*body, tr,false);
                        dof6->setLinearLowerLimit(btVector3(0,0,0));
                        dof6->setLinearUpperLimit(btVector3(0,0,0));
                        dof6->setAngularLowerLimit(btVector3(0,0,0));
                        dof6->setAngularUpperLimit(btVector3(0,0,0));
                        
                        Globals::dynamicsWorld->addConstraint(dof6);
                        m_pickConstraint = dof6;
                        
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
                        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
                        
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
                        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
                        
                        //save mouse position for dragging
                        gOldPickingPos = m_rayTo;
                        gHitPos = pickPos;
                        
                        gOldPickingDist  = (pickPos-Start).length();
                    }
                }
            }
            }
            else
            {
//                if(pickedBody){
//                    if(pickedBody->getCollisionShape()->getShapeType()== SPHERE_SHAPE_PROXYTYPE){
//                        Globals::dynamicsWorld->removeConstraint(Globals::joint_ball);
//                    }
//                }
                if (m_pickConstraint && Globals::dynamicsWorld)
                {
                    Globals::dynamicsWorld->removeConstraint(m_pickConstraint);
                    cout << "removed constraint" << endl;
                    delete m_pickConstraint;
                    //printf("removed constraint %i",gPickingConstraintId);
                    m_pickConstraint = 0;
                    pickedBody->forceActivationState(ACTIVE_TAG);
                    pickedBody->setDeactivationTime( 0.f );
                    pickedBody = 0;
                }
                
                
            }
            break;
            
    }
    lastPoint = trackBallMapping(x, y);
    lastPoint_z = Vector3(x, y, 0);
    lastPoint.print("mouse");
    
    glMatrixMode(GL_MODELVIEW);
}




void Window::renderPlane(btRigidBody* body)
{
    glColor3f(1, 1, 1);
    
    btTransform t;
    
    body->getMotionState()->getWorldTransform(t);
    
    float mat[16];
    
    t.getOpenGLMatrix(mat);
    
    glPushMatrix();
    
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 1, 0);
    
    glVertex3f(-20, 0, 20);
    
    glVertex3f(-20, 0, -20);
    
    glVertex3f(20, 0, -20);
    
    glVertex3f(20, 0, 20);
    
    glEnd();
    
    glPopMatrix();
    
}


void Window::renderSphere(btRigidBody* body)

{
    
    //printf("render ball\n");
    
    if (body->getCollisionShape()->getShapeType()!= SPHERE_SHAPE_PROXYTYPE) {
        
        printf("not sphere\n");
        
        return;
        
    }
    
    //glColor3f(0.6, 0.6, 0.6);
    
    float r = ((btSphereShape*)body->getCollisionShape())->getRadius();
    
    //printf("rad is %f\n",r);
    
    btTransform t;
    
    body->getMotionState()->getWorldTransform(t);
    
    float mat[16];
    
    t.getOpenGLMatrix(mat);
    
    glPushMatrix();
    
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    
    glutSolidSphere(r, 20, 20);
    
    //gluSolidSphere(r, 20, 20);
    
    glMatrixMode(GL_TEXTURE);
    glActiveTextureARB(GL_TEXTURE7);
    glPushMatrix();
    glMultMatrixf(mat);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}



void Window::renderCube(btRigidBody* body)

{
    
    //printf("render cube!\n");
    
    if (body->getCollisionShape()->getShapeType()!= BOX_SHAPE_PROXYTYPE) {
        
        printf("not box\n");
        
        return;
        
    }
    
    glColor3f(1, 1, 0);
    
    btVector3 extent = ((btBoxShape*)body->getCollisionShape())->getHalfExtentsWithoutMargin();
    
    //printf("rad is %f\n",r);
    
    btTransform t;
    
    body->getMotionState()->getWorldTransform(t);
    
    //printf("%f,%f,%f, ex %f,%f,%f\n",t.getOrigin().getX(),t.getOrigin().getY(),t.getOrigin().getZ(),extent.x(),extent.y(),extent.z());
    
    float mat[16];
    
    t.getOpenGLMatrix(mat);
    
    glPushMatrix();
    
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    glBegin(GL_QUADS);
    
    glNormal3f(-1, 0, 0);
    
    glVertex3f(-extent.x(), extent.y(), -extent.z());
    
    glVertex3f(-extent.x(), -extent.y(), -extent.z());
    
    glVertex3f(-extent.x(), -extent.y(), extent.z());
    
    glVertex3f(-extent.x(), extent.y(), extent.z());
    
    glEnd();
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(1, 0, 0);
    
    glVertex3f(extent.x(), extent.y(), -extent.z());
    
    glVertex3f(extent.x(), -extent.y(), -extent.z());
    
    glVertex3f(extent.x(), -extent.y(), extent.z());
    
    glVertex3f(extent.x(), extent.y(), extent.z());
    
    glEnd();
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 0, 1);
    
    glVertex3f(-extent.x(), extent.y(), extent.z());
    
    glVertex3f(-extent.x(), -extent.y(), extent.z());
    
    glVertex3f(extent.x(), -extent.y(), extent.z());
    
    glVertex3f(extent.x(), extent.y(), extent.z());
    
    glEnd();
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(-1, 0, -1);
    
    glVertex3f(-extent.x(), extent.y(), -extent.z());
    
    glVertex3f(-extent.x(), -extent.y(), -extent.z());
    
    glVertex3f(extent.x(), -extent.y(), -extent.z());
    
    glVertex3f(extent.x(), extent.y(), -extent.z());
    
    glEnd();
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, 1, 0);
    
    glVertex3f(-extent.x(), extent.y(), -extent.z());
    
    glVertex3f(-extent.x(), extent.y(), extent.z());
    
    glVertex3f(extent.x(), extent.y(), extent.z());
    
    glVertex3f(extent.x(), extent.y(), -extent.z());
    
    glEnd();
    
    
    
    glBegin(GL_QUADS);
    
    glNormal3f(0, -1, 0);
    
    glVertex3f(-extent.x(), -extent.y(), -extent.z());
    
    glVertex3f(-extent.x(), -extent.y(), extent.z());
    
    glVertex3f(extent.x(), -extent.y(), extent.z());
    
    glVertex3f(extent.x(), -extent.y(), -extent.z());
    
    glEnd();
    
    //glutSolidCube(1);
    glPopMatrix();
    
}

