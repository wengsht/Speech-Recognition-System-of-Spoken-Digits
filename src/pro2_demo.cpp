#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iostream>
#include "FeatureExtractor.h"

using namespace std;

void Load(DAEPAnalysis & da_ep) {
    load_calc("output", "da_ep", da_ep, false);
}
int main() {
    FeatureExtractor extractor;

    DAEPAnalysis da_ep;

    Load(da_ep);

    RawData * data = da_ep.data();

    extractor.exFeatures(data);

    return 0;
}
