#pragma once
#include <stdlib.h>
#include <list>

//struct allCplexModels {
//
//	allCplexModels() {
//		weightedSum = new WeightedSumModel();
//		feasibilityCheck = new FeasibilityCheckModel();
//		dualBenson = new DualBensonModel();
//		furthestFeasiblePoint = new FurthestFeasiblePointModel();
//		prob = new ProbingModel();
//		varFix = new VariableFixingModel();
//		ws = new WeightedSumModel();
//	}
//
//	~allCplexModels() {
//		delete weightedSum;
//		delete feasibilityCheck;
//		delete dualBenson;
//		delete furthestFeasiblePoint;
//		delete prob;
//		delete varFix;
//		delete ws;
//	}
//
//	void build(MathematicalModel* lp, Parameters* param) {
//		weightedSum->build(*lp);
//		feasibilityCheck->build(*lp);
//		dualBenson->build(*lp);
//		furthestFeasiblePoint->build(*lp);
//		prob->build(*lp, param);
//		varFix->build(*lp, param);
//		ws->build(*lp);
//	}
//
//	WeightedSumModel* weightedSum; //!< pointer to the cplex model that computes weithed sums
//	FeasibilityCheckModel* feasibilityCheck; //!< pointer to the cplex model that computes checks for the feasibility of a point
//	DualBensonModel* dualBenson; //!< pointer to the cplex model that computes the dual from benson's method
//	FurthestFeasiblePointModel* furthestFeasiblePoint; //!< pointer to the cplex model that computes the points on the boundary of the LB set in benson's method
//	ProbingModel* prob; //!< a pointer to the probing (cplex) model.
//	VariableFixingModel* varFix; //!< a pointer to the (cplex) model used to fix variables when probing.
//	WeightedSumModel* ws; //!< a pointer to the (cplex) model that is used to compute weighted sums,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//};

struct CoverCut {
	CoverCut() {
		idx = std::list<int>(0);
		idxx = std::vector<double>(0);
		rhs = -1;
		coefs = std::vector<std::list<int>>(0);
		threshold = std::vector<int>(0);
		isNew = true;
	}

	CoverCut(int n) {
		idx = std::list<int>(0);
		idxx = std::vector<double>(n, 0);
		rhs = -1;
		coefs = std::vector<std::list<int>>(0);
		threshold = std::vector<int>(0);
		isNew = true;
	}

	/*CoverCut(int p) {
		idx = std::list<int>(0);
		idxx = std::vector<double>(0);
		rhs = -1;
		coefs = std::vector<std::list<int>>(p, std::list<int>(0));
		threshold = std::vector<int>(p);
		isNew = true;
	}*/

	std::list<int> idx; //!< list of indices of variables in the cut
	std::vector<double> idxx; //!< vector of coefficients of variables in the cut
	int rhs; //!< rhs of the cut -> |C| - 1
	std::vector<std::list<int>> coefs;
	std::vector<int> threshold;
	bool isNew; //!< true if generated in the current node
};

struct Parameters {
	Parameters() : LBset(0), nodeSelection(0), variableSelection(0), objectiveBranching(0), branchingValueSelection(0), timeOut(3600), GCD(0), versionProbing(0), limitSubPb(0), threshold(-2), searchDir(0), selDir(0), domiVarFix(false), adjustBBWSbounds(false), generateCuts(false), pbcc(false), debug(false), FLP(false), epsOB(0), gapMeasure(-1), shift(0.0), shiftVec(0), nadir(0), ideal(0) {}

	int LBset;
	int nodeSelection;
	int variableSelection;
	int objectiveBranching;
	int branchingValueSelection;
	int timeOut;
	std::vector<int> GCD; //!< greatest common divisor for each objective
	int versionProbing;
	int limitSubPb; //!< limit on the number of subpb generated at each node with OB
	double threshold;
	std::vector<double> searchDir; //!< weights for calculating BEST_BOUND node selections. -> used in domi var fix test
	std::vector<double> selDir; //!< same but used in node sel
	int domiVarFix; //!< 0 = obj to 0, 1 = obj to 1, 2 = obj to ws
	bool adjustBBWSbounds;
	bool generateCuts;
	bool pbcc;
	bool debug;
	bool FLP; // true if FLP specific branching rule (facilities first)
	double epsOB; // shift in the LB set when computing objective branching
	int gapMeasure; // gap measure used
	double shift; // used only if gapMeasure = LB_SHIFT
	std::vector<double> shiftVec; // used only if gapMeasure = LB_SHIFT
	//std::vector<double> ubObj; //!< ub on each obj for the overall pb
	std::vector<int> nadir;
	std::vector<int> ideal;
	int enumThreshold;
};

struct BranchingDecisions {
	BranchingDecisions() {
		lb = std::vector<int>(0);
		ub = std::vector<int>(0);
		slub = std::vector<int>(0);
		lastSplittedIndex = -1;
		depth = 0;
		yIprobing = std::vector<double>(0);
		score = 0;
		cc = std::list<CoverCut>(0);
		ccMaxViolation = 0;
	}
	BranchingDecisions(int n, int p) {
		lb = std::vector<int>(n);
		ub = std::vector<int>(n);
		slub = std::vector<int>(p);
		lastSplittedIndex = -1;
		depth = -1;
		yIprobing = std::vector<double>(p); // index order: variable - bound value (0 or 1) - objective
		score = 0;
		cc = std::list<CoverCut>(0);
		ccMaxViolation = 0;
	}

	std::vector<int> lb; //!< a vector that describes the lower bounds on the variables in this node. It thus contains some of the previous branching decisions.
	std::vector<int> ub; //!< a vector that describes the upper bounds on the variables in this node. It thus contains some of the previous branching decisions.
	std::vector<int> slub; //!< a vector that describes the search cone of this node in the objective space. It corresponds to the right-hand side of the objective branching constraints.
	int lastSplittedIndex; //!< index of the last splitted index. -1 if there is no such thing (e.g. in root node)
	int depth;
	//std::vector<int> cut0; //!< a vector to remember the value of the cut when fixing variable x_i to 0.
	//std::vector<int> cut1; //!< a vector to remember the value of the cut when fixing variable x_i to 0.
	//std::list<int> resetVar; //!< list of variables to reset when doing variable fixing in probing.
	std::vector<double> yIprobing; //!< list of anti-ideal points when probing.
	double score;
	std::list<CoverCut> cc;
	int ccMaxViolation;
};

//struct LPRelaxStat {
//	LPRelaxStat() : lpSolved(0), feasibilityCheckSolved(0), furthestFeasbilePointSolved(0), dualBensonSolved(0), totalTime(), timeUpdatePolyhedron(), timeFeasibilityCheck(), timeFurthestFeasiblePoint(), timeInitialization(), profiler() {
//	}
//
//	int lpSolved; //!< total number of LP solved
//	int feasibilityCheckSolved; //!< number of calls to FeasibilityCheck model
//	int furthestFeasbilePointSolved; //!< number of calls to FurthestFeasiblePoint model
//	int dualBensonSolved; //!< number of calls to dualBenson model
//	Timer totalTime;
//	Timer timeUpdatePolyhedron; //!< time spent in updating the polyhedron
//	Timer timeFeasibilityCheck; //!< time spent in checking whether extreme points are feasible and computing pre-images
//	Timer timeFurthestFeasiblePoint; //!< time spent in computing the furthest feasible point given an infeasible point
//	Timer timeDualBenson; //!< time spent in solving the dual problem for finding the new hypeplanes
//	Timer timeInitialization; //!< time spent in initializing the polyhedron
//
//	Timer profiler;
//};

// status
const int UNSOLVED = 0;
const int SOLVED = 1;
const int INFEASIBLE = 2;
const int OPTIMAL = 3;
const int UNBOUNDED = 4;
const int DOMINATED = 5;

// others
const int IS_LB = 10;
const int IS_UB = 11;

// LB sets
const int LP_RELAX = 100; // implemented
const int WARMSTARTED_LP_RELAX = 101; // implemented
const int IMPLICIT_LP_RELAX = 102;
const int PRECOMPUTED_WARMSTARTED_LP_RELAX = 103; // implemented for OB only, the LB set is computed and UB set updated at the creation of the node

// node selection
const int DEPTH_FIRST = 200; // implemented
const int BREADTH_FIRST = 201; // implemented
const int SCORE_BASED = 202;
const int HYBRID = 203;
const int BEST_BOUND_WS = 204;
const int MOST_FRACTIONAL = 205;
const int BEST_BOUND_MAXMIN_GAP = 206;
const int BEST_BOUND_GAP = 207;
//const int BEST_BOUND_GAP2 = 208;

// variable selection
const int FIRST_INDEX = 300; // implemented
const int MOST_OFTEN_FRACTIONAL = 301; // implemented
const int PROBING = 302; // implemented, variable fixing with brute force approach (test each value for each free variable)
//const int PROBING_CUTS = 303;
const int PROBING_PRESOLVE_LP = 304;
const int PROBING_PRESOLVE_IP = 305;
const int VARIABLE_FIXING = 306; // variable fixing with as little lp solved as possible

// objective branching
const int NO_OBJECTIVE_BRANCHING = 400; // implemented
const int CONE_OBJECTIVE_BRANCHING = 401; // implemented
const int FULL_OBJECTIVE_BRANCHING = 402; // implemented
const int LIMITED_OBJECTIVE_BRANCHING = 403;

// branching value selection
const int MEDIAN = 500; // implemented
const int MOST_OFTEN_FRACTIONAL_VALUE = 501; // implemented
const int RANDOM = 502;

// version probing
const int OLD_RULE = 600;
const int CLOSEST_DIMENSION = 601;
//const int WORST_BEST_YI = 602;
const int SCORE_WS = 603;

// gap measure
const int LB_SHIFT = 700;
const int HAUSDORFF = 701;

// -----------------------------------
// epsilons & big M
const double M = 999999989;
const double EPS_LAMBDA = 0.00001;
const double EPS_PROXIMITY = 0.000001;
const double EPS_INT = 0.000001;

// debug
const double PRINT_DEBUG = -1;
const double DEBUG_IT = -1; // 4143
const int CORRECTION_WARMSTART = 7000;

// timers
const int TIME_OUT = 3600;
const int TIME_OUT_LB = 3600;

// parameters variable branching
const double MU = 0; // should be between 0 and 1

// triggers
const bool ACTIVATE_DOMINANCE_VARIABLE_FIXING = true;
const bool ENABLE_LB_SEARCH_IN_BEST_BOUND = true;
const bool ADJUST_SEARCH_DIR_TO_COEF = true;
const bool ACTIVATE_NO_GOOD_CUTS = false;
const bool ENABLE_EARLY_LB_DOMI = false;
const bool PTS_BASED_COVER_CUT = true;

// threshold enum
const int ENUM_THRESHOLD = 14; // 14	-1