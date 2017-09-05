#ifndef GUI_QT
#define GUI_QT

#include <iostream>
#include <memory>
#include <graph_qt.h>
#include <gui_graph_interface.hpp>
#include <fnn_math.hpp>

namespace gui {

    class GuiGraphQt : public GuiGraphInterfaceExt, public GuiGraphInterfaceInt {
        std::unique_ptr<gui_qt::GraphQt> m_graph;

        std::string m_graphName;

        const mw::DataVector* m_dataV;

        std::size_t m_dataLen;

        double m_dataMax;

        double m_dataMin;

        std::set<int> m_mask;

        void getMinMaxStock(double& min, double& max) const {
            auto res = math::compute<math::Limits>(*m_dataV, m_mask);
            min = res.first;
            max = res.second;
        }

        void testDataPresence() const {
            try {
                std::cout << "Test of data presence on vector 0" << std::endl;
                auto& element = m_dataV->getElementAt(0);
                for(const auto& m : m_mask) {
                    auto name = m_dataV->getNameAt(m);
                    auto val = element.dataAt(m);
                    std::cout << "#" << m << ": " << name << "  " << val << std::endl;
                }
                std::cout << std::endl;
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(
                        std::string("[2] Chosen dataset index exceeds range. ") +
                        std::string("Maybe your input file lacks some columns? '") +
                        e.what() + std::string("'"));
            }
        }


        public:
        GuiGraphQt()
            : m_graph(new gui_qt::GraphQt(this)) {
        }


        //External interface
        virtual void setData(const mw::DataVector* const dataV, const std::set<int>& mask, const std::string& graphName) {
            m_graphName = graphName;
            m_mask = mask;
            m_dataV = dataV;
            m_dataLen = m_dataV->getElementsSize();

            testDataPresence();
            getMinMaxStock(m_dataMin, m_dataMax);

            m_graph->update();

            std::cout << "File:\t\t" << m_graphName << std::endl;
            std::cout << "Data length:\t" << m_dataLen << std::endl;
            std::cout << "Data max:\t" << m_dataMax << std::endl;
            std::cout << "Data min:\t" << m_dataMin << std::endl << std::endl;
        }

        virtual void show() const {
            m_graph->show();
        }


        //Internal interface
        virtual std::size_t graphDataLength() const {
            return m_dataLen;
        }

        virtual double graphDataMax() const {
            return m_dataMax;
        }

        virtual double graphDataMin() const {
            return m_dataMin;
        }

        virtual const std::string& graphName() const {
            return m_graphName;
        }

        virtual const std::set<int>& dataMask() const {
            return m_mask;
        }

        virtual const mw::DataVector& dataVector() const {
            return *m_dataV;
        }


    };
}

#endif
