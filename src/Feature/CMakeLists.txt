set(FEATURE Feature)
set(MFCC_CUDA MfccCuda)

add_library(${FEATURE} Feature.cpp FeatureExtractor.cpp mathtool.cpp WaveFeatureOP.cpp WordDtwRecognition.cpp WaveFeatureOPSet.cpp 
    HMMRecognition.cpp HMMAutomatonSet.cpp HMMKMeanAutomaton.cpp
    HMMSoftAutomaton.cpp
    HMMAutomaton.cpp
    KMeanState.cpp
    SoftState.cpp
    HMMState.cpp
	Gaussian.cpp
    HMMSeqRecognition.cpp
    SeqModel.cpp
    NoEmitState.cpp
    HMMSeqTrainer.cpp
    HMMSeqKMeanTrainer.cpp
    wtime.cpp
	)


cuda_add_library(${MFCC_CUDA} Feature.cpp FeatureExtractorTool.cu FeatureExtractor.cu mathtool.cpp WaveFeatureOP.cpp WordDtwRecognition.cpp WaveFeatureOPSet.cpp 
    HMMRecognition.cpp HMMAutomatonSet.cpp HMMKMeanAutomaton.cpp
    HMMSoftAutomaton.cpp
    HMMAutomaton.cpp
    KMeanState.cpp
    SoftState.cpp
    HMMState.cpp
	Gaussian.cpp
    HMMSeqRecognition.cpp
    SeqModel.cpp
    NoEmitState.cpp
    HMMSeqTrainer.cpp
    HMMSeqKMeanTrainer.cpp
    wtime.cpp
    )
