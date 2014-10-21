# quick start 

./Feature
├── CMakeLists.txt
├── Feature.cpp
├── Feature.h
├── FeatureExtractor.cpp
├── FeatureExtractor.h
├── HMMAutomaton.cpp
├── HMMAutomaton.h           ; HMM 自动机的基类
├── HMMAutomatonSet.cpp      ; 存储多个templates的自动机
├── HMMAutomatonSet.h
├── HMMKMeanAutomaton.h      ; KMean 自动机
├── HMMKmeanAutomaton.cpp
├── HMMRecognition.cpp
├── HMMRecognition.h         ; 前端， 提供 train(), recognition(wav_feature) 接口
├── HMMSoftAutomaton.cpp
├── HMMSoftAutomaton.h       ; Soft 自动机
├── HMMState.cpp
├── HMMState.h               ; State 基类
├── KMeanState.cpp
├── KMeanState.h             ; KMean State ， 提供gaussTrain() nodeCost() 接口
├── SoftState.cpp
├── SoftState.h              ; Soft State, 同上
├── WaveFeatureOP.cpp
├── WaveFeatureOP.h          ; 在HMM中充当存储一个wav(template) 的feature的作用，调用  .size() 和 [] 遍历它
├── WaveFeatureOPSet.cpp
├── WaveFeatureOPSet.h       ; 模板库, HMM中的AutomatonSet 继承自它， 方便从目录中load 数据
├── WordDtwRecognition.cpp
├── WordDtwRecognition.h     ; Part 1 前端
├── mathtool.cpp
└── mathtool.h


## Gaussian 

分成了两个类， KMean 有的信息是一个vector<WaveFeatureOP> *ptr, 和vector< pair<int, int> > edgePoints;

其中 ptr vector 每一个元素对应一个模板wav， 而edgePoints 存储对应模板在这个state的开始下标和结束下标 

Soft 有的信息是一个vector<WaveFeatureOP> *ptr, 和Matrix<double> 概率;

## 测试驱动

!!!!  记得在./templates 下面录一个wav
* ./hmm_demo -d input(.wav)
hmm_demo.cpp:

    68 hmm.setStateType(HMMState::KMEAN);
    69 //hmm.setStateType(HMMState::SOFT);

可以设置algorithm

* configure_hmm 可以设置默认的gaussian分段数

* KMEAN 在平均分段的时候调用了一次train
* Soft 在平均分布的时候（概率都是1.0/stateNumber) 调用一次train

 写成两个类是为了节约KMEAN分段的开销。。你可以试试写个Gauss类之类的把他们结合一下，能给我提供借口就行









