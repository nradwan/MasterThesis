#ifndef LMALGR_H
#define LMALGR_H

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <math.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h> 
#include <limits>

class LMAlgr{
	
	public:
		//constructor
		LMAlgr (Eigen::MatrixXf observations, Eigen::MatrixXf params, Eigen::MatrixXf &pose);
		~LMAlgr();

	private:
		double threshold;
		double outlier_threshold;
		
		void init(Eigen::MatrixXf observations, Eigen::MatrixXf params, Eigen::MatrixXf &pose);
		void computeIncrement(Eigen::MatrixXf params, Eigen::MatrixXf pose, double lambda, Eigen::MatrixXf error, Eigen::MatrixXf &delta);
		double computeError(Eigen::MatrixXf observations, Eigen::MatrixXf params, Eigen::MatrixXf pose, Eigen::MatrixXf &error);
		double normalize_angle(double angle);
		void computeLM(Eigen::MatrixXf observations, Eigen::MatrixXf params, Eigen::MatrixXf &pose);
};

#endif
