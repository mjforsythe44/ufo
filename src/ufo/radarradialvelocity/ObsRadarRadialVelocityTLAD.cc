/*
 * (C) Copyright 2017-2018 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "ufo/radarradialvelocity/ObsRadarRadialVelocityTLAD.h"

#include <ostream>

#include "ioda/ObsSpace.h"
#include "ioda/ObsVector.h"
#include "oops/base/Variables.h"
#include "oops/util/Logger.h"
#include "ufo/GeoVaLs.h"
#include "ufo/ObsBias.h"
#include "ufo/ObsBiasIncrement.h"

namespace ufo {

// -----------------------------------------------------------------------------
static LinearObsOperatorMaker<ObsRadarRadialVelocityTLAD>
                              makerRadarRadialVelocityTL_("RadarRadialVelocity");
// -----------------------------------------------------------------------------

ObsRadarRadialVelocityTLAD::ObsRadarRadialVelocityTLAD(const ioda::ObsSpace & odb,
                               const eckit::Configuration & config)
  : keyOper_(0), varin_(), odb_(odb)
{
  const eckit::Configuration * configc = &config;
  const oops::Variables & observed = odb.obsvariables();
  const eckit::Configuration * varconfig = &observed.toFortran();
  ufo_radarradialvelocity_tlad_setup_f90(keyOper_, &configc, &varconfig, varin_);

  oops::Log::trace() << "ObsRadarRadialVelocityTLAD created" << std::endl;
}

// -----------------------------------------------------------------------------

ObsRadarRadialVelocityTLAD::~ObsRadarRadialVelocityTLAD() {
  ufo_radarradialvelocity_tlad_delete_f90(keyOper_);
  oops::Log::trace() << "ObsRadarRadialVelocityTLAD destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsRadarRadialVelocityTLAD::setTrajectory(const GeoVaLs & geovals, const ObsBias & bias) {
  ufo_radarradialvelocity_tlad_settraj_f90(keyOper_, geovals.toFortran(), odb_);
  oops::Log::trace() << "ObsRadarRadialVelocityTLAD: trajectory set" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsRadarRadialVelocityTLAD::simulateObsTL(const GeoVaLs & geovals, ioda::ObsVector & ovec,
                             const ObsBiasIncrement & bias) const {
  ufo_radarradialvelocity_simobs_tl_f90(keyOper_, geovals.toFortran(), odb_,
                            ovec.size(), ovec.toFortran());
  oops::Log::trace() << "ObsRadarRadialVelocityTLAD: TL observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsRadarRadialVelocityTLAD::simulateObsAD(GeoVaLs & geovals, const ioda::ObsVector & ovec,
                             ObsBiasIncrement & bias) const {
  ufo_radarradialvelocity_simobs_ad_f90(keyOper_, geovals.toFortran(), odb_,
                            ovec.size(), ovec.toFortran());
  oops::Log::trace() << "ObsRadarRadialVelocityTLAD: adjoint observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsRadarRadialVelocityTLAD::print(std::ostream & os) const {
  os << "ObsRadarRadialVelocityTLAD::print not implemented" << std::endl;
}

// -----------------------------------------------------------------------------

}  // namespace ufo