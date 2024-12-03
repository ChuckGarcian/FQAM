
#define FQAM_SUCCESS           (-1)
#define FQAM_FAILURE           (-2)

typedef int FQAM_Error;

typedef struct
{
  int m;
  int n;
  double angle;
  int eigen_value;
} FQAM_Basis;

typedef struct 
{
  char name [10];    // Operator Name    
  FLA_Obj mat_repr;  // Operator Matrix Representation
  int dimension;     // Hilbert Dimension 
  int mat_repr_initialized;
  int initialized; 

  // Outer Objects
  bool outer;
  FQAM_Basis outer_ket0;  
  FQAM_Basis outer_ket1;  
} FQAM_Op;  
