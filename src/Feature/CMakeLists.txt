set(FEATURE Feature)
add_library(${FEATURE} Feature.cpp FeatureExtractor.cpp mathtool.cpp WaveFeatureOP.cpp WordDtwRecognition.cpp WaveFeatureOPSet.cpp 
    HMMRecognition.cpp
    HMMAutomatonSet.cpp
    HMMKMeanAutomaton.cpp
    HMMSoftAutomaton.cpp
    HMMAutomaton.cpp
    KMeanState.cpp
    SoftState.cpp
    HMMState.cpp
	Gaussian.cpp
	)
