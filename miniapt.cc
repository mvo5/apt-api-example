//
// build with:
//   g++ -o miniapt miniapt.cc -lapt-pkg
//
#include <apt-pkg/cachefile.h>

static pkgPolicy *policy;

static void print_candidate(const pkgCache::PkgIterator &P)
{
   pkgCache::VerIterator current = P.CurrentVer();
   pkgCache::VerIterator candidate = policy->GetCandidateVer(P);

   if(!candidate || !candidate.FileList())
      return;
   
   std::string archive, origin, component;
   pkgCache::VerFileIterator VF = candidate.FileList();
   // see InRelease for the fields
   // also available: Codename, Label
   if (VF.File().Archive())
      archive = VF.File().Archive();
   if(VF.File().Origin())
      origin = VF.File().Origin();
   if(VF.File().Component())
      component = VF.File().Component();

   if (candidate) {
      std::cout << P.FullName() << " "
                << candidate.VerStr() << " "
                << archive << " "
                << origin << " "
                << component << " "
                << std::endl;
   }
   // FIXME: add examle about how to access the pkgRecord via
   // pkgTagFile etc
}


int main(int argc,const char **argv)
{
    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);

    pkgCacheFile cachefile;
    pkgCache *cache = cachefile.GetPkgCache();
    policy = cachefile.GetPolicy();

    if (cache == NULL || _error->PendingError()) {
        _error->DumpErrors();
        return 1;
    }

    for (pkgCache::GrpIterator grp = cache->GrpBegin(); grp != cache->GrpEnd(); grp++)
       for (pkgCache::PkgIterator p = grp.PackageList(); !p.end(); p = grp.NextPkg(p))
          print_candidate(p);
}
