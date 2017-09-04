#ifndef GUI_INTERFACE
#define GUI_INTERFACE

namespace gui {

    class GuiGraphInterfaceExt {
        public:
        virtual void foo() = 0;
    };

    class GuiGraphInterfaceInt {
        public:
        virtual void bar() = 0;
    };

}

#endif
