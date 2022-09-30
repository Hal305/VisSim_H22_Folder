#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>
#include <unordered_map>
#include <ctime>

#include "vertex.h"
#include "visualobject.h"
#include "camera.h"
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "light.h"
#include "xyz.h"
#include "trianglesurface.h"
#include "octaball.h"
#include "rollingball.h"
#include "raindrop.h"
#include "barycentriccalc.h"
#include "bsplinecurve.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
//    std::vector<int> t {0,0,0,1,2,2,3};
//    std::vector<QVector2D> c {{0,1},{1,0},{2,1},{3,0},{4,1}};
//    BSplineCurve spline(2, t, c);
//    qDebug() << spline.evaluateBSplineSample(1);

    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a Cube
void RenderWindow::init()
{
    srand(time(NULL));
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));

    //Print info about opengl texture limits on this GPU:
    int textureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  This GPU as ") + std::to_string(textureUnits) + std::string(" texture units / slots in total, \n");

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    tempString += std::string("  and supports ") + std::to_string(textureUnits) + std::string(" texture units pr shader");

    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //set up alpha blending for textures
    glEnable(GL_BLEND);// you enable blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaders.push_back(new Shader("../VisSim_H22_Folder/plainshader.vert", "../VisSim_H22_Folder/plainshader.frag"));
    mLogger->logText("Plain shader program id: " + std::to_string(mShaders.back()->getProgram()) );
    mShaders.push_back( new Shader("../VisSim_H22_Folder/PhongShader.vert", "../VisSim_H22_Folder/PhongShader.frag"));
    mLogger->logText("Texture shader program id: " + std::to_string(mShaders.back()->getProgram()) );

    for(unsigned int i = 0; i < mShaders.size(); i++)
    setupShader(i);

    mMMatrix = new QMatrix4x4{};
    mMMatrix->setToIdentity();    //1, 1, 1, 1 in the diagonal of the matrix

    mCamera = new Camera();

    //Standard/Quadtree
    VisualObject* temp = new XYZ();
    temp->setName("xyz");
    mObjects.push_back(temp);

    surface = new TriangleSurface("../VisSim_H22_Folder/terrain.txt");
    mObjects.push_back(surface);
    ball = new RollingBall(3);
    dynamic_cast<RollingBall*>(ball)->setSurface(surface);
    mObjects.push_back(ball);

    mLight = new Light(mShaders[0]->getProgram());
    mLight->setName("light");
    mLight->mMatrix.translate(1.f, 1.f, 1.f);
    mObjects.push_back(mLight);

    //Quadtree init
//    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4};
//    mQuadTree.init(a, b, c, d);
//    mQuadTree.subDivide(1);
//    auto subtree = mQuadTree.find(gsml::Point2D(2,2));
//    subtree->subDivide(1);
//    for (auto it : mObjects)
//        mQuadTree.insert(it->getPosition2D(), it->getName(), it);
//    mQuadTree.printAll();

    aspectratio = static_cast<float>(width()) / height();
    mCamera->init();
    mCamera->perspective(60.f, aspectratio, 0.1f, 400.f);
    //Camera position
    mCamera->lookAt(QVector3D{0,-2, 5}, QVector3D{0,0,0}, QVector3D{0,0,1});

    //standard container
    for(auto it : mObjects)
    {
        if (it->getShaderId()==mShaders[1]->getProgram())
        {
            it->init(mMMatrixUniform[1]);
        }
        else
        {
            it->init(mMMatrixUniform[0]);
        }
    }

    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

void RenderWindow::setupShader(int index)
{
    mMMatrixUniform.push_back(glGetUniformLocation(mShaders[index]->getProgram(), "mMatrix"));
    mVMatrixUniform.push_back(glGetUniformLocation(mShaders[index]->getProgram(), "vMatrix"));
    mPMatrixUniform.push_back(glGetUniformLocation(mShaders[index]->getProgram(), "pMatrix"));
    if(index == 2)
        {
            mLightColorUniform = glGetUniformLocation( mShaders[index]->getProgram(), "lightColor" );
            mObjectColorUniform = glGetUniformLocation( mShaders[index]->getProgram(), "objectColor" );
            mAmbientLightStrengthUniform = glGetUniformLocation( mShaders[index]->getProgram(), "ambientStrength" );
            mLightPositionUniform = glGetUniformLocation( mShaders[index]->getProgram(), "lightPosition" );
            mSpecularStrengthUniform = glGetUniformLocation( mShaders[index]->getProgram(), "specularStrength" );
            mSpecularExponentUniform = glGetUniformLocation( mShaders[index]->getProgram(), "specularExponent" );
            mLightPowerUniform = glGetUniformLocation( mShaders[index]->getProgram(), "lightPower" );
            mCameraPositionUniform = glGetUniformLocation( mShaders[index]->getProgram(), "cameraPosition" );
            mTextureUniform2 = glGetUniformLocation(mShaders[index]->getProgram(), "textureSampler");
        }
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //the actual draw call
    //standard container, works with quadtree init
    //    if(!bCameraLock)

    for(auto it : mObjects){
        if((*it).getShaderId()==mShaders[1]->getProgram())
        {
            glUseProgram(mShaders[1]->getProgram());
            glUniformMatrix4fv(mVMatrixUniform[1], 1, GL_FALSE, mCamera->mVMatrix.constData());
            glUniformMatrix4fv(mPMatrixUniform[1], 1, GL_FALSE, mCamera->mPMatrix.constData());
            //            glUniform1i(mTextureUniform, 1);
            glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(), mLight->mMatrix.column(3).z());
            glUniform3f(mCameraPositionUniform, mCamera->position().x(), mCamera->position().y(), mCamera->position().z());
            glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
            glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrength);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, (*it).getTexId());
        }
        else
        {
            glUseProgram(mShaders[0]->getProgram());
            glUniformMatrix4fv(mVMatrixUniform[0], 1, GL_FALSE, mCamera->mVMatrix.constData());
            glUniformMatrix4fv(mPMatrixUniform[0], 1, GL_FALSE, mCamera->mPMatrix.constData());
        }
        if(bWireFrame)
            it->drawLines();
        else
            it->draw();
    }
    static float rotate{0.f};
    mLight->mMatrix.translate(sinf(rotate)/10, cosf(rotate)/10, cosf(rotate)/60);
    rotate += 0.01f;

    if(bRaining)
    {
        rainTimer += 0.5f;
        if(rainTimer >= 3 && rainDropCount < 20)
        {
            x = rand() % 60/10.f, y = rand() % 20/10.f;
            //qDebug() << x << y;
            mRaindrops.push_back(new RainDrop(r, x, y));
            rainDropCount ++;
            mRaindrops.back()->init(mMMatrixUniform[0]);
            rainTimer = 0;
        }
        if(!mRaindrops.empty())
        {
            rainFall();
        }
    }
    else
    {
        if(!mRaindrops.empty())
        {
            rainFall();
        }
        else
        {
            rainTimer = 0;
            rainDropCount = 0;
        }
    }

    movePlayer();
    ball->move(0.017f);

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::rainFall()
{
    for (auto it : mRaindrops)
    {
        it->draw();
        if (it->getZ() <= 0)
        {
            mRaindrops.erase(mRaindrops.begin());
            rainDropCount--;
        }
    }
    for (auto it : mRaindrops)
        it->move(0.005f);
}

void RenderWindow::movePlayer()
{
    float d = 0.2f;
    if(!bCameraLock)
    {
        if(mInput.W == true)
        {
            mCamera->translate(0.f, -d, 0);
        }
        if(mInput.A == true)
        {
            if(!bCameraLock)
                mCamera->translate(d, 0.f, 0);
        }
        if(mInput.S == true)
        {
            mCamera->translate(0.f, d, 0);
        }
        if(mInput.D == true)
        {
            mCamera->translate(-d, 0.f, 0);
        }

        if(mInput.Q == true)
        {
            mCamera->translate(0.f, 0.f, -d*2);
        }
        if(mInput.E == true)
        {
            mCamera->translate(0.f, 0.f, d*2);
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{

    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    //You get the keyboard input like this
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        mInput.D = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }

    if(event->key() == Qt::Key_Space)
    {
//        npc->setPath();
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
}
