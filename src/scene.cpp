#include "scene.h"

//Constructor, empty scene
Scene::Scene(){};

//adds shape to the scene
void Scene::add_object(Object &o){
    objects.push_back(o);
}

//open a minwin window and sets its parameters (for instance,title and size)
void Scene::initialise(){
    window.set_title( "Rasterizer Cazin Florent" );
    window.set_width( screen_resolution[0] );
    window.set_height( screen_resolution[1] );
}

void Scene::unload_data(){
    this->objects.clear();
}


void Scene::shutdown(){
    window.close();
    unload_data();
}

//Converts viewport coordinates of a point to canvas coordinates.
Vec2r Scene::viewport_to_canvas( const Vec2r & point ) const{
    Vec2r canevas_coord;
    canevas_coord[0] = (point[0] * (static_cast<real>(screen_resolution[0])/viewport_resolution[0]));
    canevas_coord[1] = (point[1] * (static_cast<real>(screen_resolution[1])/viewport_resolution[1])); // /hauteur
    return canevas_coord;
}

//Converts canvas coordinates of a point to window (screen) coordinates.
Vec2i Scene::canvas_to_window( const Vec2r & point ) const{
    Vec2i screen_coord;
    screen_coord[0] = static_cast<int>((screen_resolution[0] / 2.0) + point[0]);
    screen_coord[1] = static_cast<int>((screen_resolution[1] / 2.0) - point[1]); // Inversion de l'axe y
    return screen_coord;
}


void Scene::draw_line(const Vec2r &v0, const Vec2r &v1) const{

    Vec2i v0Tmp = canvas_to_window(viewport_to_canvas(v0));
    Vec2i v1Tmp = canvas_to_window(viewport_to_canvas(v1));

    int x0 = v0Tmp.at(0);
    int y0 = v0Tmp.at(1);
    int x1 = v1Tmp.at(0);
    int y1 = v1Tmp.at(1);
    
    int dx = x1-x0;
    int dy = y1-y0;

    real a;
    real x,y;
    if(std::abs(dx) > std::abs(dy)){
        if(dx==0){ //avoid div 0
            a= std::copysign(1.0,dy);
        }
        else{
            if(x0>x1){
                swap(x0,x1);
                swap(y0,y1);
            }
            a = static_cast<real>(dy)/dx;
        }
        y=y0;
        for(int xVar=x0;xVar<=x1;++xVar){
            window.put_pixel(xVar,std::round(y),this->color);
            y = y + a;
        }
    }
    //if line is vertical
    else{
        if(dy==0){ //avoid div 0
            a=std::copysign(1.0,dx);
        }
        else{
            if(y0>y1){
                swap(x0,x1);
                swap(y0,y1);
            }
            a=static_cast<real>(dx)/dy;
        }
        x=x0;
        for(int yVar=y0;yVar<=y1;++yVar){
            window.put_pixel(std::round(x),yVar,this->color);
            x = x + a;
        }
    }
}

vector<int> Scene::interpolate(int i0,int d0,int i1,int d1) const{
    if(i0==i1) return vector<int>{d0};
    real a = (static_cast<real>(d0-d1)) / (i0-i1);
    real d = d0;
    vector<int> values;
    for(int i=i0;i<=i1;++i){
        values.push_back(std::round(d));
        d+=a;
    }
    return values;
}

void Scene::draw_filled_triangle(const Vec2r &v0, const Vec2r &v1, const Vec2r &v2) const{
    Vec2i p0 = canvas_to_window(viewport_to_canvas(v0)); //p0
    Vec2i p1 = canvas_to_window(viewport_to_canvas(v1)); //p1
    Vec2i p2 = canvas_to_window(viewport_to_canvas(v2)); //p2

    
    //to know what is the top and the bottom of the triangle, y0<=Y1<=y2
    if(p1[1]<p0[1]) swap(p1,p0);
    if(p2[1]<p0[1]) swap(p2,p0);
    if(p2[1]<p1[1]) swap(p2,p1);

    //p0
    int x0 = p0.at(0);
    int y0 = p0.at(1);
    //p1
    int x1 = p1.at(0);
    int y1 = p1.at(1);
    //p2
    int x2 = p2.at(0);
    int y2 = p2.at(1);

    //tall side (p0,p2), short side is concatenation of (p0,p1) and (p1,p2)
    //we want one value of x for each value of y
    vector<int> x02 = interpolate(y0,x0,y2,x2); //tall side values
    vector<int> x01 = interpolate(y0,x0,y1,x1);
    vector<int> x12 = interpolate(y1,x1,y2,x2);
    //concatenate so repeated value for one pixel
    x01.pop_back();
    x01.insert(x01.end(),x12.begin(),x12.end()); 
    vector<int> x012 = x01;
    //now we must determine which of the arrays is left and which is right
    //check coordinate at middle of the triangle
    int m = floor(static_cast<real>(x012.size())/2);
    vector<int> x_left;
    vector<int> x_right;
    if(x02[m]<x012[m]){
        x_left = x02;
        x_right = x012;
    }
    else{
        x_left = x012;
        x_right = x02;
    }
    //draw the horizontal lines
    for(int y=y0;y<=y2;++y){
        for(int x=x_left[y-y0]; x<=x_right[y-y0];++x){
            window.put_pixel(x,y,this->color);
        }
    }
}

void Scene::draw_shaded_triangle(const Vec2r &v0, real h0, const Vec2r &v1, real h1, const Vec2r &v2, real h2) const{

    Vec2i p0 = canvas_to_window(viewport_to_canvas(v0)); //p0
    Vec2i p1 = canvas_to_window(viewport_to_canvas(v1)); //p1
    Vec2i p2 = canvas_to_window(viewport_to_canvas(v2)); //p2

    //p0
    int x0 = p0.at(0);
    int y0 = p0.at(1);
    //p1
    int x1 = p1.at(0);
    int y1 = p1.at(1);
    //p2
    int x2 = p2.at(0);
    int y2 = p2.at(1);

    //compute values of h on the edges of the triangle
    vector<int> x01 = interpolate(y0,x0,y1,x1);
    vector<int> h01 = interpolate(y0,h0,y1,h1);

    vector<int> x12 = interpolate(y1,x1,y2,x1);
    vector<int> h12 = interpolate(y1,h1,y2,h1);

    vector<int> x02 = interpolate(y0,x0,y2,x2);
    vector<int> h02 = interpolate(y0,h0,y2,h2);


    //concatenate the two parts of the short side
    x01.pop_back();
    x01.insert(x01.end(),x12.begin(),x12.end()); 
    vector<int> x012 = x01;

    h01.pop_back();
    h01.insert(h01.end(),h12.begin(),h12.end());
    vector<int> h012 = h01;

    //which is left and which is right
    int m = floor(static_cast<real>(x012.size())/2);
    vector<int> x_left,x_right,h_left,h_right;
    if(x02[m] < x012[m]){
        x_left = x02;
        h_left = h02;

        x_right=x012;
        h_right=h012;
    }
    else{
        x_left = x012;
        h_left = h012;

        x_right=x02;
        h_right=h02;
    }

    for(int y=y0; y<=y2;++y){
        int x_l = x_left[y-y0];
        //int x_r = x_right[y-y0];

        vector<int> h_segment = interpolate(x_left[y-y0], h_left[y-y0], x_right[y-y0], h_right[y-y0]);
        for(int x=x_left[y-y0]; x<=x_right[y-y0]; ++x){
            Color shaded_color{static_cast<Uint8>((this->color.r)*h_segment [x - x_l]),static_cast<Uint8>((this->color.g)*h_segment [x - x_l]),static_cast<Uint8>((this->color.b)*h_segment [x - x_l]),static_cast<Uint8>(0)};
            window.put_pixel(x,y,shaded_color);
        }
    }
}


vector<Object>& Scene::getObjects(){
    return this->objects;
}

Vec2r Scene::perspective_projection(const Vec4r &v, real d){
    Vec2r projection;

    //d is negatif here (axe z inversed)

    //avoid division by zero
    if(v[2] + d != 0){
        projection[0] = (v[0] * d) / (v[2] + d);
        projection[1] = (v[1] * d) / (v[2] + d);
    }
    //else return {0,0}
    return projection;
}



void Scene::run(){
    initialise();

    // create and register behaviors
    window.register_quit_behavior( new QuitButtonBehavior( *this ) );
    window.register_key_behavior( minwin::KEY_ESCAPE, new QuitKeyBehavior( *this ) );
    window.register_key_behavior( minwin::KEY_SPACE, new FilledTriangleBehavior( *this ) ); //press space
    window.register_key_behavior( minwin::KEY_LEFT, new LeftRotation( *this ) ); //press left
    window.register_key_behavior( minwin::KEY_RIGHT, new RightRotation( *this ) ); //press right
    window.register_key_behavior( minwin::KEY_UP, new UpRotation( *this ) ); //press up
    window.register_key_behavior( minwin::KEY_DOWN, new DownRotation( *this ) ); //press down




    // setup text

    filledText.set_pos( 50, 10 );
    filledText.set_string( "Press ESPACE to un/filled the object." );
    filledText.set_color( this->color );

    moveText.set_pos( 50, 30 );
    moveText.set_string( "Use UP,DOWN,LEFT,RIGHT to move." );
    moveText.set_color( this->color );

    quitText.set_pos( 50, 50 );
    quitText.set_string( "Press ESC to quit." );
    quitText.set_color( this->color );

    

    // load font
    if( not window.load_font( "fonts/FreeMonoBold.ttf", 16u ) )
    {
        std::cerr << "Couldn't load font.\n";
    }

    // open window
    if( not window.open() )
    {
        std::cerr << "Couldn't open window.\n";
        return;
    }

    vector<Object> &objectScene = getObjects();
    vector<Vertex> verticesShape;
    vector<Face> facesShape;
    
    while( this->running )
    {
        // process keyboard inputs, etc.
        window.process_input();
        
        // clear window
        window.clear();

        // set color
        window.set_draw_color( this->color );

        // draw text
        window.render_text( quitText );
        window.render_text( moveText );
        window.render_text( filledText );


        //drawing all objects
        for(Object &object : objectScene){
            facesShape= object.getShape()->get_faces();
            verticesShape = object.getShape()->get_vertices();
            Matrix<real,4,4> transformMatrix = object.transform();
            for(Face &face : facesShape){
                this->color = face.color;
                //Apply transformation to each vertex
                Vec4r transform_v0 = transformMatrix * Vec4r{verticesShape[face.v0].c3d[0],verticesShape[face.v0].c3d[1],verticesShape[face.v0].c3d[2],1.0};
                Vec4r transform_v1 = transformMatrix * Vec4r{verticesShape[face.v1].c3d[0],verticesShape[face.v1].c3d[1],verticesShape[face.v1].c3d[2],1.0};
                Vec4r transform_v2 = transformMatrix * Vec4r{verticesShape[face.v2].c3d[0],verticesShape[face.v2].c3d[1],verticesShape[face.v2].c3d[2],1.0};

                //Perform perspective projection on transformed vertices
                Vec2r proj_v0 = perspective_projection(transform_v0,2);
                Vec2r proj_v1 = perspective_projection(transform_v1,2);
                Vec2r proj_v2 = perspective_projection(transform_v2,2);



                if(this->filledTriangle==true){
                    draw_filled_triangle(proj_v0,proj_v1,proj_v2);
                    //draw_shaded_triangle(verticesShape[face.v0].c,verticesShape[face.v0].h,verticesShape[face.v1].c,verticesShape[face.v1].h,verticesShape[face.v2].c,verticesShape[face.v2].h);
                }
                else{
                    draw_line(proj_v0,proj_v1);
                    draw_line(proj_v1,proj_v2);
                    draw_line(proj_v2,proj_v0);
                }
            }
        }
        
        // display elements drawn so far
        window.display();
        //break;
    }

    // shutdown and close window
    shutdown();
}

//BINDING
void Scene::QuitButtonBehavior::on_click() const { this->owner.running = false; }
void Scene::QuitKeyBehavior::on_press() const   { this->owner.running = false; }
void Scene::QuitKeyBehavior::on_release() const {} // does nothing

void Scene::FilledTriangleBehavior::on_press() const {
    if(this->owner.filledTriangle==true) this->owner.filledTriangle=false;
    else this->owner.filledTriangle=true;
}
void Scene::FilledTriangleBehavior::on_release() const {}

void Scene::LeftRotation::on_press() const {
    for(Object &object : this->owner.objects){
        object.makeLeftRotation(0.1);
    }
}
void Scene::LeftRotation::on_release() const {}

void Scene::RightRotation::on_press() const {
    for(Object &object : this->owner.objects){
        object.makeLeftRotation(-0.1);
    }
}
void Scene::RightRotation::on_release() const {}

void Scene::UpRotation::on_press() const {
    for(Object &object : this->owner.objects){
        object.makeUpRotation(-0.1);
    }
}
void Scene::UpRotation::on_release() const {}

void Scene::DownRotation::on_press() const {
    for(Object &object : this->owner.objects){
        object.makeUpRotation(0.1);
    }
}
void Scene::DownRotation::on_release() const {}


void Scene::load_shape(char *file){

    size_t length = strlen(file);
    if (length > 4) {
        char lastFour[5];
        strcpy(lastFour, file + length - 4);
        if (strcmp(lastFour, ".obj") != 0) {
            std::cerr << "Error opening file: " << file << "\n";
        }
    }
    else{
        std::cerr << "Error opening file: " << file << "\n";
    }

    //open the file
    std::ifstream objFile(file);
    if (!objFile.is_open()) {
        std::cerr << "Error opening file: " << file << "\n";
    }
    vector<Vertex> vertices;
    vector<Face> faces;
    string line;

    real v1,v2,v3;
    int i1,i2,i3;
    Color colorArray[] = {BLUE,RED,GREEN,GREY,WHITE,CYAN,MAGENTA,YELLOW};
    size_t ite=0;
    //read the file
    while(std::getline(objFile,line)){
        //vertex
        if(line.substr(0,2) == "v "){
            std::sscanf(line.c_str(), "v %lf %lf %lf", &v1,&v2,&v3);
            Vec3r c{v1,v2,v3};
            Vertex vertex(c,1.0);
            vertices.push_back(vertex);
        }
        //face
        else if(line.substr(0,2) == "f "){
            std::sscanf(line.c_str(), "f %d %d %d", &i1,&i2,&i3);
            if(ite==size(colorArray)) ite=0;
            Face face(i1-1,i2-1,i3-1, colorArray[ite++]);
            faces.push_back(face);
        }
    }
    Shape shape(file,vertices,faces);
    Object object(shape,Vec3r{0.0,0.0,-8.0},Vec3r{0.0,0.0,3.15},Vec3r{1.0,1.0,1.0});
    add_object(object);
}

int main(int argc, char* argv[])
{

    Scene scene;
    
    if(argc<2){

    //construct shapes
        minwin::Color  colorBlue  { minwin::BLUE };
        minwin::Color  colorRed  { minwin::RED };
        Vec3r v0{-0.1, -0.1, 0.0}, v1{0.2, -0.2, 0.0}, v2{0.2, 0.2, 0.0}, v3{-0.2, 0.2, 0.0}, v4{-0.9, -0.3, 0.0}, v5{-0.24, -0.05, 0.0}, v6{-0.68, 0.3, 0.0};
        Vertex vertex0{v0,1.0}, vertex1{v1,1.0}, vertex2{v2,1.0}, vertex3{v3,1.0}, vertex4{v4,1.0}, vertex5{v5,1.0}, vertex6{v6,1.0};
        std::vector<Vertex> verticesShape1 = {vertex0,vertex1,vertex2,vertex3};
        std::vector<Vertex> verticesShape2 = {vertex4,vertex5,vertex6};
        Face f1{0,1,2,colorBlue},f2{0,2,3,colorRed},f3{0,1,2,colorBlue};
        std::vector<Face> facesTmpShape1 = {f1,f2};
        std::vector<Face> facesTmpShape2 = {f3};
        Shape shapeScene1{"pyramid",verticesShape1,facesTmpShape1};
        Shape shapeScene2{"triangle",verticesShape2,facesTmpShape2};
        Object o1{shapeScene1,Vec3r{0.0,0.0,-5.0},Vec3r(),Vec3r{1.0,1.0,1.0}};
        Object o2{shapeScene2,Vec3r{0.0,0.0,-5.0},Vec3r(),Vec3r{1.0,1.0,1.0}};


        //init scene
        scene.add_object(o1);
        scene.add_object(o2);
    }
    else{
        for(int i=1;i<argc;i++){
            scene.load_shape(argv[i]);
        }
    }
    

    //start
    scene.run();
 
    // return 0 means OK.*/
    return 0;
}

