set(FEATURE Feature)
add_library(${FEATURE} Feature.cpp FeatureExtractor.cpp mathtool.cpp WaveFeatureOP.cpp WordDtwRecognition.cpp WaveFeatureOPSet.cpp)
