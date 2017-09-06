#ifndef GUI_PROGRESS_INTERFACE
#define GUI_PROGRESS_INTERFACE


namespace gui {

    class GuiProgressInterfaceExtSync {
        public:
        virtual void updateProgress(int value) = 0;
    };

    class GuiProgressInterfaceExt {
        public:
        virtual void show() const = 0;
    };

    class GuiProgressInterfaceInt {
        public:
        virtual int getValue() const = 0;
    };

}

#endif
