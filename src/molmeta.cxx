#include <gauxc/molmeta.hpp>

namespace GauXC {

MolMeta::MolMeta( const Molecule& mol ) : natoms_(mol.natoms()){
  compute_rab(mol);
  compute_dist_nearest();
}

MolMeta::MolMeta( const MolMeta& ) = default;
MolMeta::MolMeta( MolMeta&& ) noexcept = default;
MolMeta::~MolMeta() noexcept = default;


void MolMeta::compute_rab(const Molecule& mol) {

  rab_.resize( natoms_*natoms_ );

  for( int32_t i = 0; i < natoms_; ++i ) {
    rab_[i*(natoms_+1)] = 0.;
    for( int32_t j = 0; j < i; ++j ) {
      const double dab_x = mol[i].x - mol[j].x;
      const double dab_y = mol[i].y - mol[j].y;
      const double dab_z = mol[i].z - mol[j].z;

      rab_[i + j*natoms_] = std::sqrt(dab_x*dab_x + dab_y*dab_y + dab_z*dab_z);
      rab_[j + i*natoms_] = rab_[i + j*natoms_];
    }
  }

}

void MolMeta::compute_dist_nearest() {

  dist_nearest_.resize(natoms_);
  for( int32_t i = 0; i < natoms_; ++i ) {
    double dn = std::numeric_limits<double>::infinity();

    auto at_begin = rab_.begin() + i*natoms_;
    for( int32_t j = 0; j < natoms_; ++j )
    if( i != j and *(at_begin + j) < dn )
      dn = *(at_begin + j);

    dist_nearest_[i] = dn;
  }

}

}