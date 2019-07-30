
#include <vector>

#ifndef PLNR_VECTORN_H
#define PLNR_VECTORN_H

// from and to where is possible to include metrics in the vectorn_flags structure i.e., LOWER_VECTORN_FLAGS_LIMIT <= metric < UPPER_VECTORN_FLAGS_LIMIT
#define LOWER_VECTORN_FLAGS_LIMIT   100
#define UPPER_VECTORN_FLAGS_LIMIT   200
#define VECTORN_FLAGS_GAIN                30
#define VECTORN_FLAGS_LOSS                -30

namespace plnr
{
    enum struct vectorn_flags { 
        unflagged = 0, 
        
        gain =          VECTORN_FLAGS_GAIN,
        loss =          VECTORN_FLAGS_LOSS, // gain says how much a value differs from power (i.e., energy - power = gain). It's for purely implementation details

        power =         LOWER_VECTORN_FLAGS_LIMIT,
        power_cpu =     LOWER_VECTORN_FLAGS_LIMIT + 1,
        power_gpu =     LOWER_VECTORN_FLAGS_LIMIT + 2,

        energy =        LOWER_VECTORN_FLAGS_LIMIT + VECTORN_FLAGS_GAIN,
        energy_cpu =    LOWER_VECTORN_FLAGS_LIMIT + VECTORN_FLAGS_GAIN + 1,
        energy_gpu =    LOWER_VECTORN_FLAGS_LIMIT + VECTORN_FLAGS_GAIN + 2,

        soc =           LOWER_VECTORN_FLAGS_LIMIT + 2*VECTORN_FLAGS_GAIN,
        soc_cpu =       LOWER_VECTORN_FLAGS_LIMIT + 2*VECTORN_FLAGS_GAIN + 1,
        soc_gpu =       LOWER_VECTORN_FLAGS_LIMIT + 2*VECTORN_FLAGS_GAIN + 2,

        problem_dimension = UPPER_VECTORN_FLAGS_LIMIT, // numbers in [200,300) are for problem's dimensions (i.e., for the arguments of a component). Use formulation vectorn_flags::problem_dimension + x s.t. 0 <= x < 100
    };

    /// Gives a point in n-dimensional space. The class stores a sequence of real numbers and defines operators enabling to treat such sequences as real vectors
    class vectorn {

    private:
        int                                     _length;
        std::vector<double>                     _vector;
        std::vector<std::vector<vectorn_flags>> _flags;

        void _vectorn(int __length, double* values, std::vector<std::vector<vectorn_flags>> __flags);

    public:
        vectorn(int __length, double* values, std::vector<std::vector<vectorn_flags>> __flags);

        /// @brief Given a set of doubles and its length n, initializes a nx1 vector
        /// @param __length vector's length
        /// @param values   a set of doubles
        vectorn(int __length, double* values);

        vectorn(int __length, std::vector<std::vector<vectorn_flags>> __flags);

        /// @brief Given a double, initializes a 1x1 vector
        /// @param value    value to assign to 1x1 vector
        vectorn(double value);

        /// @brief Given an int n, initializes a nx1 void vector
        /// @param __length number of vector components
        vectorn(int __length);

        /// @brief Copy constructor
        /// @param _vectorn object to be copied
        vectorn(const vectorn& _vectorn);

        /// @brief Default constructor
        vectorn();

        /// @brief Default destructor
        ~vectorn();
        
        /// @brief Gives vector's size
        /// @return value of vectors size
        const int length() const;

        /// @brief Gives the value stored in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @return         the value stored in the row index
        const double get(int index) const;

        /// @brief Gives the value stored in a specific row in a nx1 vector
        /// @param flag     flag of the row to be returned
        /// @return         the value stored in the row with a specific flag
        const double get(vectorn_flags flag) const;

        /// @brief Gives the index of the vectorn corresponding to a specific flag
        /// @param flag     flag of which the index has to be obtained
        /// @return         the index corresponding to a specific flag
        const int get_index(vectorn_flags flag) const;

        /// @brief Gives the flags of the vectorn corresponding to a specific index
        /// @param index    index of which the flag has to be obtained
        /// @return         the set of falgs corresponding to a specific index
        std::vector<vectorn_flags> get_flag(int index);

        std::vector<std::vector<vectorn_flags> > get_flags();

        /// @brief Stores a vaue in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @param value    the value to be stored in the row index
        void set(int index, double value);

        /// @brief Stores a vaue in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @param value    the value to be stored in the row index
        /// @param flag     the flag with which a specific row has to be flagged
        void set(int index, double value, vectorn_flags flag);

        /// @brief Stores a vaue in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @param value    the value to be stored in the row index
        /// @param _flags   flags set with which a specific row has to be flagged
        void set(int index, double value, std::vector<vectorn_flags> _flags);

        /// @brief Sets a flag in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @param flag     the flag with which a specific row has to be flagged
        void set_flag(int index, vectorn_flags flag);

        /// @brief Sets a set of flags in a specific row in a nx1 vector
        /// @param index    index of the row
        /// @param __flags  flags set with which a specific row has to be flagged
        void set_flag(int index, std::vector<vectorn_flags> __flags);

        /// @brief Says whenever a specific index contains the desired flag
        /// @param index    index of the row
        /// @param flag     the flag to be checked
        /// @return         boolean value that says if flag is contained
        bool has_flag(int index, vectorn_flags flag);

        /// @brief Remove a flag from the flags list of a specific row of the vectorn
        /// @param index    index of the row
        /// @param flag     the flag to be removed
        void remove_flag(int index, vectorn_flags flag);

        /// @brief Inherits all flags from another vectorn (i.e. the flag on row i of the vectorn _vectorn, will be the same as the flag on row i of the vectorn this)
        /// @param _vectorn the object which flags have to be inherited by the current instance
        /// @return         the current instance this
        vectorn* inherit_flags(vectorn _vectorn);

        void empty_flags();

        void transform_flags(vectorn_flags flag);

        /// @brief Gives the Euclidean norm of the vector
        /// @return the value of the Euclidean norm
        double abs();

        /// @brief Gives a copy of itself
        /// @return a pointer to a  object containing the copy of the current one
        vectorn* copy();

        vectorn& operator=(const vectorn& _vectorn);
        vectorn operator+(const vectorn& _vectorn) const;
        vectorn operator-(const vectorn& _vectorn) const;
        vectorn operator+(const double value) const;
        vectorn operator-(const double value) const;
        vectorn operator*(const double value) const;
        vectorn operator/(const double value) const;

        friend vectorn operator*(const double lhs, const vectorn& rhs) { return rhs.operator*(lhs); }
        friend vectorn operator/(const double lhs, const vectorn& rhs) { return rhs.operator/(lhs); } 
    };  
}

#endif
