#ifndef GUI_GRAPH_QT
#define GUI_GRAPH_QT

#include <iostream>
#include <memory>
#include <mutex>
#include <logger.h>
#include <graph_qt.h>
#include <gui_graph_interface.hpp>
#include <fnn_math.hpp>

namespace gui {

    class GuiGraphQt : public GuiGraphInterfaceExtSync,
                        public GuiGraphInterfaceExt,
                        public GuiGraphInterfaceInt {

        struct _shared {
            std::string graphName;
            std::set<int> mask;
            mw::DataVector dataV;
        };


        std::unique_ptr<gui_qt::GraphQt> m_graph;

        _shared m_shared;

        std::mutex m_dataMtx;


        std::string m_graphName;

        std::set<int> m_mask;

        mw::DataVector m_dataV;

        std::size_t m_dataLen;

        double m_dataMax;

        double m_dataMin;


        void getMinMaxStock(double& min, double& max) const {
            auto res = math::compute<math::Limits>(m_dataV, m_mask);
            min = res.first;
            max = res.second;
        }

        void testDataPresence() const {
            try {
                tout << "Test of data presence on vector 0" << std::endl;
                auto& element = m_dataV.getElementAt(0);
                for(const auto& m : m_mask) {
                    auto name = m_dataV.getNameAt(m);
                    auto val = element.dataAt(m);
                    tout << "#" << m << ": " << name << "  " << val << std::endl;
                }
                tout << std::endl;
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(
                        std::string("[2] Chosen dataset index exceeds range. ") +
                        std::string("Maybe your input file lacks some columns? '") +
                        e.what() + std::string("'"));
            }
        }

        void setSharedData(const mw::DataVector& dataV, const std::set<int>& mask, const std::string& graphName) {
            std::unique_lock<std::mutex> lck(m_dataMtx);

            m_shared.graphName = graphName;
            m_shared.mask = mask;
            m_shared.dataV = dataV;
        }

        void getSharedData() {
            std::unique_lock<std::mutex> lck(m_dataMtx);

            m_graphName = m_shared.graphName;
            m_mask = m_shared.mask;
            m_dataV = m_shared.dataV;
        }

        void notifyGraph() const {
            emit m_graph->signalNewData();
        }


        public:
        GuiGraphQt()
            : m_graph(new gui_qt::GraphQt(this)) {
            tout << __func__ << "(), this: " << this << std::endl;
        }

        virtual ~GuiGraphQt() {
            tout << __func__ << "(), this: " << this << std::endl;
        }


        //External synchronized interface
        virtual void setData(const mw::DataVector& dataV, const std::set<int>& mask, const std::string& graphName) override {
            setSharedData(dataV, mask, graphName);
            notifyGraph();
        }


        //External interface
        virtual void show() const override {
            m_graph->show();
        }


        //Internal interface
        virtual std::size_t graphDataLength() const override {
            return m_dataLen;
        }

        virtual double graphDataMax() const override {
            return m_dataMax;
        }

        virtual double graphDataMin() const override {
            return m_dataMin;
        }

        virtual const std::string& graphName() const override {
            return m_graphName;
        }

        virtual const std::set<int>& dataMask() const override {
            return m_mask;
        }

        virtual const mw::DataVector& dataVector() const override {
            return m_dataV;
        }

        virtual void newDataInit() override {
            getSharedData();

            m_dataLen = m_dataV.getElementsSize();

            testDataPresence();
            getMinMaxStock(m_dataMin, m_dataMax);

            tout << "Graph name:\t\t" << m_graphName << std::endl;
            tout << "Data length:\t" << m_dataLen << std::endl;
            tout << "Data max:\t" << m_dataMax << std::endl;
            tout << "Data min:\t" << m_dataMin << std::endl << std::endl;
        }


    };
}

#endif
