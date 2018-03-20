#pragma once

//#include "MathUtil_Matrix.h"
//#include "MathUtil_Rotator.h"
//#include "MathUtil_vector3.h"

namespace DvUtils
{
   namespace Math
   {
	    // Camera-related math
        //
        //  If the matrix is created with
        //    Matrix.CreatePerspectiveFieldOfView(fov,aspectRatio,
        //                                        nearClipPlaneDist,farClipPlaneDist)
        //  
        //  -Z is negative into the screen   
        //  -then the following functions may be used
        //
        //
        // perspective projection info struct
        struct PerspectiveProjInfo
        {
            // fields
            float FOV;// field of view in radians
            float aspectRatio;// width to height aspect ratio
            // construction
            PerspectiveProjInfo(float FOVIn, float aspectRatioIn) : FOV(FOVIn), aspectRatio(aspectRatioIn) {}
       };

	   //Matrix WorldMatrix (const Vec<float, 3>& location, const Rotator& rotation);
       //Matrix ViewMatrix  (const Vec<float, 3>& location, const Rotator& rotation);
       //Matrix ProjectionMatrix(const PerspectiveProjInfo& proj, float znear, float zfar);
   } // namespace Math
} // namespace DvUtils