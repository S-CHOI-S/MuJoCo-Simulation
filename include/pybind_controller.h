#pragma once
#ifndef __PYBIND_CONTROLLER_H
#define __PYBIND_CONTROLLER_H

#include "pybind11/pybind11.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
// #include <iostream>
#include <eigen3/Eigen/Dense>
#include <rbdl/rbdl.h>
#include <rbdl/addons/urdfreader/urdfreader.h>

#include "robotmodel.h"
#include "trajectory.h"
#include "custommath.h"

using namespace std;
using namespace Eigen;

#define NECS2SEC 1000000000

class MJCController
{

public:
    MJCController();
    virtual ~MJCController();	

    // Functions (pybind)
    void read(double t, std::array<double, 9> q, std::array<double, 9> qdot);
    void control_mujoco(std::array<double, 3> des_position);
    std::array<double, 9> write();
    bool check_joint_limit(std::array<double, 9> q);
    bool check_velocity_limit();

    VectorXd _q, _qdot, _q_order, _qdot_order;
    
    void Initialize();

private:
    
    void ModelUpdate();
    void motionPlan();
    void reset_target(double motion_time, VectorXd target_pose);

    VectorXd _torque, _pre_q, _pre_qdot; // joint torque
    int _k; // DOF
    
    std::array<double, 9> torque;

    bool _bool_init;
    double _t;
    double _dt;
    double _init_t;
    double _pre_t;

    //controller
    double _kpj, _kdj; //joint P,D gain
    double _x_kp; // task control P gain
    double _x_kd; // task control D gain

    void JointControl();
    void CLIK();
    void OperationalSpaceControl();

    // robotmodel
    CModel Model;

    int _cnt_plan;
	VectorXd _time_plan;
	VectorXi _bool_plan;

    int _control_mode; //1: joint space, 2: operational space
    VectorXd _q_home; // joint home position

    //motion trajectory
    double _start_time, _end_time, _motion_time;

    CTrajectory JointTrajectory; // joint space trajectory
    HTrajectory HandTrajectory; // task space trajectory

    bool _bool_joint_motion, _bool_ee_motion; // motion check

    VectorXd _q_des, _qdot_des; 
    VectorXd _q_goal, _qdot_goal;
    VectorXd _x_des_hand, _xdot_des_hand;
    VectorXd _x_goal_hand, _xdot_goal_hand;
    Vector3d _pos_goal_hand, _rpy_goal_hand;

    MatrixXd _A_diagonal; // diagonal inertia matrix
    MatrixXd _J_hands; // jacobian matrix
    MatrixXd _J_bar_hands; // pseudo invere jacobian matrix
    MatrixXd _J_T_hands; // jacobian transpose matrix
    MatrixXd _J_bar_T_hands; // jacobian inverse transpose matrix

    VectorXd _x_hand, _xdot_hand; // End-effector

    VectorXd _x_err_hand;
    VectorXd _x_dot_err_hand;
    Matrix3d _R_des_hand;
    
    // For Operational Space Control
    MatrixXd _I; // Identity matrix
    MatrixXd _J_null; // Null space control jacobian matrix
    MatrixXd _Lambda; // Inertia matirx: Operational Space
    VectorXd F_command_star; // command vector of the decoupled end-effector
    
    // For joint limit
    VectorXd _max_joint_position;
    VectorXd _min_joint_position;
};

#endif
