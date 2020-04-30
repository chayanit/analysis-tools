// Original code: CMSSW_10_2_22 - JetMETCorrections/Modules/src/JetResolution.cc

#include "Analysis/Tools/interface/JetResolution.h"

// These two includes are needed to run standalone
#include <algorithm>
#include <iostream>

namespace JME {

    JetResolution::JetResolution(const std::string& filename) {
        m_object = std::make_shared<JetResolutionObject>(filename);
    }

    JetResolution::JetResolution(const JetResolutionObject& object) {
        m_object = std::make_shared<JetResolutionObject>(object);
    }

    float JetResolution::getResolution(const JetParameters& parameters) const {
        const JetResolutionObject::Record* record = m_object->getRecord(parameters);
        if (! record)
            return 1;

        return m_object->evaluateFormula(*record, parameters);
    }

    JetResolutionScaleFactor::JetResolutionScaleFactor(const std::string& filename) {
        m_object = std::make_shared<JetResolutionObject>(filename);
    }

    JetResolutionScaleFactor::JetResolutionScaleFactor(const JetResolutionObject& object) {
        m_object = std::make_shared<JetResolutionObject>(object);
    }

    float JetResolutionScaleFactor::getScaleFactor(const JetParameters& parameters,
                                                   Variation variation /* = Variation::NOMINAL*/,
                                                   std::string uncertaintySource /* = ""*/) const {
        const JetResolutionObject::Record* record = m_object->getRecord(parameters);
        if (! record)
            return 1;

        const std::vector<float>& parameters_values = record->getParametersValues();
        const std::vector<std::string>& parameter_names = m_object->getDefinition().getParametersName();
        size_t parameter = static_cast<size_t>(variation);
        if (!uncertaintySource.empty()) {
          if (variation == Variation::DOWN)
            parameter = std::distance(parameter_names.begin(),
                                      std::find(parameter_names.begin(), parameter_names.end(), uncertaintySource + "Down"));
          else if (variation == Variation::UP)
            parameter = std::distance(parameter_names.begin(),
                                      std::find(parameter_names.begin(), parameter_names.end(), uncertaintySource + "Up"));
          if (parameter >= parameter_names.size()) {
            std::string s;
            for (const auto& piece : parameter_names)
              s += piece + " ";
              // Replacing the throw cms::Exception below
              std::cerr << "ERROR in JetResolution: "
               << "Invalid value for 'uncertaintySource' parameter. Only " + s + " are supported.\n" ;
              throw std::exception();
//            throw cms::Exception("InvalidParameter") << "Invalid value for 'uncertaintySource' parameter. Only " + s + " are supported.\n";
          }
        }
        return parameters_values[parameter];
    }

}
