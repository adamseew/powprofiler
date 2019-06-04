
#include "vectorn.hpp"
#include "config.hpp"

#ifndef PLNR_INTEGRATOR_RK4_H
#define PLNR_INTEGRATOR_RK4_H

namespace plnr
{
    /// Abstract class that gives the value of the first derivative from an ODE
    class first_derivative {

    public:
        virtual ~first_derivative() { };

        /// @brief Used to obtain the value of the first derivative of a desired function
        /// @param x    value of the independent variable, i.e. time
        /// @param y    value of the dependent variable
        /// @return     the first derivative
        virtual vectorn get_value(double x, vectorn y) = 0;
    };

    /// Gives one step of the fourth-order Runge-Kutta method to sovle a first order differential equation with initial conditions
    class integrator_rk4 {

    private:
        double t0, h;
        first_derivative* _first_derivative;
        vectorn* y0;
        vectorn* dy0;

    public:

        integrator_rk4(config* _config, first_derivative* __first_derivative, double _t0, vectorn _y0, vectorn _dy0);

        /// @brief Initialize an integrator object with initial conditions
        /// @param __first_derivative   object that represents the first derivative of a desired function
        /// @param _t0                  value of the indipedent variable, i.e. time
        /// @param _y0                  value of the dependent variable
        /// @param _h                   integration step, i.e. time interval
        /// @param _dy0                 value of its derivative, i.e. dy0/dt
        integrator_rk4(first_derivative* __first_derivative, double _t0, vectorn _y0, double _h,  vectorn _dy0);

        /// @brief Default destructor
        ~integrator_rk4();

        /// @brief Gives values (params are passed as pointers) after one step of the fourth-order Runge-Kutta method
        /// @param t1   value of the indipedent variable, i.e. time after the end of the step
        /// @param y1   value of the dependent variable
        /// @param dy1  value of its derivative, i.e. dy1/dt
        void step(double* t1, vectorn* y1, vectorn* dy1);
    };
}

#endif
