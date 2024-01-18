#include <vector>
#include "shape.h"
#include "window.h"
#include "ibehavior.h"
#include "color.h"
#include "text.h"
#include "fstream"
#include "object.h"
using namespace std;

class Scene{

    public:
        Scene();
        void run();
        void shutdown();
        void initialise();
        void add_object(Object &o);
        void draw_line( const Vec2r & v0, const Vec2r & v1 ) const;
        void draw_filled_triangle(const Vec2r &v0, const Vec2r &v1, const Vec2r &v2) const;
        void draw_shaded_triangle(const Vec2r &v0, real h0, const Vec2r &v1, real h1, const Vec2r &v2, real h2) const;
        vector<Object> &getObjects();
        vector<int> interpolate(int i0,int d0,int i1,int d1) const;
        Vec2r viewport_to_canvas( const Vec2r & point ) const;
        Vec2i canvas_to_window( const Vec2r & point ) const;
        void load_shape(char *file);
        bool           filledTriangle { false };
        void unload_data();
        Vec2r perspective_projection(const Vec4r &v, real d);

    private:
        minwin::Window window;
        minwin::Color  color  { minwin::WHITE };
        std::vector<Object> objects;
        minwin::Text   quitText, moveText, filledText;
        bool           running { true };
        //1920x1080 , 1024,768 //ubuntu=1920,977
        Vec2i screen_resolution = {1024,768};
        Vec2r viewport_resolution{2,(static_cast<real>(screen_resolution[1])/screen_resolution[0])*2};

    //-------------------------------------------------------
    //method from minwin
    class QuitButtonBehavior : public minwin::IButtonBehavior
    {
        public:
        QuitButtonBehavior( Scene & scene ) : owner { scene } {}
        void on_click() const override;
        private:
        Scene & owner;
    };
    
    class QuitKeyBehavior : public minwin::IKeyBehavior
    {
        public:
        QuitKeyBehavior( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };
    
    class ChangeColorBehavior : public minwin::IKeyBehavior
    {
        public:
        ChangeColorBehavior( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    class FilledTriangleBehavior : public minwin::IKeyBehavior
    {
        public:
        FilledTriangleBehavior( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    class LeftRotation : public minwin::IKeyBehavior
    {
        public:
        LeftRotation( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    class RightRotation : public minwin::IKeyBehavior
    {
        public:
        RightRotation( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    class UpRotation : public minwin::IKeyBehavior
    {
        public:
        UpRotation( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    class DownRotation : public minwin::IKeyBehavior
    {
        public:
        DownRotation( Scene & scene ) : owner { scene } {}
        void on_press() const override;
        void on_release() const override;
        private:
        Scene & owner;
    };

    

};