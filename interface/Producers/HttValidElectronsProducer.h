
#pragma once

#include "Artus/KappaAnalysis/interface/Producers/ValidElectronsProducer.h"

#include "HiggsAnalysis/KITHiggsToTauTau/interface/HttTypes.h"


/**
   \brief GlobalProducer, for valid electrons.
   
   Required config tags in addtion to the ones of the base class:
   - ElectronIDType
*/

class HttValidElectronsProducer: public ValidElectronsProducer<HttTypes>
{

public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::global_setting_type global_setting_type;
	typedef typename HttTypes::setting_type setting_type;

	enum class ElectronIDType : int
	{
		NONE  = -1,
		SUMMER2013LOOSE = 0,
		SUMMER2013TIGHT = 1,
	};
	static ElectronIDType ToElectronIDType(std::string const& electronIDType)
	{
		if (electronIDType == "summer2013loose") return ElectronIDType::SUMMER2013LOOSE;
		else if (electronIDType == "summer2013tight") return ElectronIDType::SUMMER2013TIGHT;
		else return ElectronIDType::NONE;
	}

	virtual void InitGlobal(global_setting_type const& globalSettings) ARTUS_CPP11_OVERRIDE;
	virtual void InitLocal(setting_type const& settings) ARTUS_CPP11_OVERRIDE;


protected:

	// Htautau specific additional definitions
	virtual bool AdditionalCriteria(KDataElectron* electron, event_type const& event,
	                                product_type& product) const  ARTUS_CPP11_OVERRIDE;


private:
	ElectronIDType electronIDType;

	float chargedIsoVetoConeSizeEB = 0.0;
	float chargedIsoVetoConeSizeEE = 0.0;
	float neutralIsoVetoConeSize = 0.0;
	float photonIsoVetoConeSizeEB = 0.0;
	float photonIsoVetoConeSizeEE = 0.0;
	float deltaBetaIsoVetoConeSize = 0.0;
	
	float chargedIsoPtThreshold = 0.0;
	float neutralIsoPtThreshold = 0.0;
	float photonIsoPtThreshold = 0.0;
	float deltaBetaIsoPtThreshold = 0.0;
	
	float isoSignalConeSize = 0.0;
	float deltaBetaCorrectionFactor = 0.0;
	float isoPtSumOverPtThresholdEB = 0.0;
	float isoPtSumOverPtThresholdEE = 0.0;
	
	bool IsMVANonTrigElectronHttSummer2013(KDataElectron* electron, bool tightID) const;
};

