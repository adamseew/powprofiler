
#include <vector>

#ifndef PLNR_VECTORN_H
#define PLNR_VECTORN_H

namespace plnr
{
    enum class vectorn_flags { 
        unflagged =     0, 

        power = 1
    };

    /// Gives a point in n-dimensional space. The class stores a sequence of real numbers and defines operators enabling to treat such sequences as real vectors
    class vectorn {

    private:
        int                                     _length;
        std::vector<double>                     _vector;
        std::vector<std::vector<vectorn_flags>> _flags;

    public:
        /// @brief Given a set of doubles and its length n, initializes a nx1 vector
        /// @param __length vector's length
        /// @param values   a set of doubles
        vectorn(int __length, double* values);

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

        /// @brief Says whenever a specific index contains the desired flas
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

        /// @brief Gives the Euclidean norm of the vector
        /// @return the value of the Euclidean norm
        double abs();

        /// @brief Gives a copy of itself
        /// @return a pointer to a  object containing the copy of the current one
        vectorn* copy();

        vectorn& operator=(const vectorn& _vectorn);
        vectorn operator+(const vectorn& _vectorn) const;
        vectorn operator-(const vectorn& _vectorn) const;
        vectorn operator*(const double value) const;
        vectorn operator/(const double value) const;

        friend vectorn operator*(const double lhs, const vectorn& rhs) { return rhs.operator*(lhs); }
        friend vectorn operator/(const double lhs, const vectorn& rhs) { return rhs.operator/(lhs); } 
    };  
}

#endif
