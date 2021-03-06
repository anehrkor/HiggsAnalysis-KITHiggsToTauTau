
#include "HiggsAnalysis/KITHiggsToTauTau/interface/Producers/RefitVertexSelector.h"



void RefitVertexSelectorBase::Init(setting_type const& settings)
{
	ProducerBase<HttTypes>::Init(settings);

	// add possible quantities for the lambda ntuples consumers

	// refitted PV coordinates and parameters
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVx", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->position.x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->position.y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->position.z() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVchi2", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->chi2 : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVnDOF", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->nDOF : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVnTracks", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->nTracks : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmaxx", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(0,0) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmayy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(1,1) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmazz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(2,2) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmaxy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(0,1) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmaxz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(0,2) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVsigmayz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPV != nullptr) ? (product.m_refitPV)->covariance.At(1,2) : DefaultValues::UndefinedFloat);
	});
	
	// refitted (w/ BS constraint) PV coordinates and parameters
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSx", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->position.x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->position.y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->position.z() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSchi2", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->chi2 : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSnDOF", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->nDOF : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSnTracks", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->nTracks : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmaxx", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(0,0) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmayy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(1,1) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmazz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(2,2) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmaxy", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(0,1) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmaxz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(0,2) : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refitPVBSsigmayz", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refitPVBS != nullptr) ? (product.m_refitPVBS)->covariance.At(1,2) : DefaultValues::UndefinedFloat);
	});

	// track ref point coordinates
	// lepton1
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP1x", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP1 != nullptr) ? (product.m_refP1)->x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP1y", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP1 != nullptr) ? (product.m_refP1)->y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP1z", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP1 != nullptr) ? (product.m_refP1)->z() : DefaultValues::UndefinedFloat);
	});
	// lepton2
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP2x", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP2 != nullptr) ? (product.m_refP2)->x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP2y", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP2 != nullptr) ? (product.m_refP2)->y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("refP2z", [](event_type const& event, product_type const& product)
	{
		return ((product.m_refP2 != nullptr) ? (product.m_refP2)->z() : DefaultValues::UndefinedFloat);
	});

	// track momentum coordinates
	// lepton1
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track1p4x", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track1p4 != nullptr) ? (product.m_track1p4)->x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track1p4y", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track1p4 != nullptr) ? (product.m_track1p4)->y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track1p4z", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track1p4 != nullptr) ? (product.m_track1p4)->z() : DefaultValues::UndefinedFloat);
	});
	// lepton2
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track2p4x", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track2p4 != nullptr) ? (product.m_track2p4)->x() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track2p4y", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track2p4 != nullptr) ? (product.m_track2p4)->y() : DefaultValues::UndefinedFloat);
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("track2p4z", [](event_type const& event, product_type const& product)
	{
		return ((product.m_track2p4 != nullptr) ? (product.m_track2p4)->z() : DefaultValues::UndefinedFloat);
	});
	
}


void RefitVertexSelectorBase::Produce(event_type const& event, product_type& product,
										setting_type const& settings) const
{
	
	assert(product.m_flavourOrderedLeptons.size() > 0);

	// create hashes from lepton selection
	std::vector<KLepton*> leptons = product.m_flavourOrderedLeptons;
	std::vector<size_t> hashes;

	if (leptons.size() == 2 && event.m_refitVertices && event.m_refitBSVertices){
		
		size_t hash = 0;

		// get reference point of the track
		product.m_refP1 = &((leptons.at(0))->track.ref);
		product.m_refP2 = &((leptons.at(1))->track.ref);

		// get momentum of the track
		product.m_track1p4 = &((leptons.at(0))->track.p4);
		product.m_track2p4 = &((leptons.at(1))->track.p4);

		for (auto lepton : leptons){
			boost::hash_combine(hash, lepton->internalId);
		} // for over leptons
		hashes.push_back(hash);

		std::swap(leptons.at(0), leptons.at(1));
		hash = 0;
		for (auto lepton : leptons){
			boost::hash_combine(hash, lepton->internalId);
		}
		hashes.push_back(hash);


	} // if leptons.size==2


	// find the vertex among the refitted vertices
	for (std::vector<KRefitVertex>::iterator vertex = event.m_refitVertices->begin(); vertex != event.m_refitVertices->end(); ++vertex){
		if ( std::find(hashes.begin(), hashes.end(), vertex->leptonSelectionHash) != hashes.end() ){
			product.m_refitPV = &(*vertex);
			break;
		}
	} // loop over refitted vertices collection


	// find the vertex among the refitted vertices calculated w/ beamspot constraint
	for (std::vector<KRefitVertex>::iterator vertex = event.m_refitBSVertices->begin(); vertex != event.m_refitBSVertices->end(); ++vertex){
		if ( std::find(hashes.begin(), hashes.end(), vertex->leptonSelectionHash) != hashes.end() ){
			product.m_refitPVBS = &(*vertex);
			break;
		}

	} // loop over refitted vertices collection

}


std::string RefitVertexSelector::GetProducerId() const
{
	return "RefitVertexSelector";
}


void RefitVertexSelector::Init(setting_type const& settings)
{
	RefitVertexSelectorBase::Init(settings);
}


void RefitVertexSelector::Produce(event_type const& event, product_type& product,
									setting_type const& settings) const
{
	RefitVertexSelectorBase::Produce(event, product, settings);
}
