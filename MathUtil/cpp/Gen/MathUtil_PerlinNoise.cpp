#include "../../h/Gen/MathUtil_PerlinNoise.h"

namespace DvUtils
{
	namespace Math
	{
		// Compute combined influence value:
		// u, v, w: coordinates [0.0; 1.0F] of point within the grid cell
		double ComputeCombinedInfluenceValue(double influence_values[2][2][2], double u, float v, double w);
		// Compute given combined influence value for plane with the given Z:
		// InZ: zero or one
		double ComputeCombinedInfluenceValue_ForGivenZ(double influence_values[2][2][2], int InZ, double u, float v);
		// Interpolate influence values using the appropriate interpolation function
		double InterpolateInfluenceValues(double InA, double InB, double w);

		// Get gradient for given arbitrary value (modulo is applied to get gardient index)
		void OutputResultGradient(Vec<double, 3> *pOutGradient, int InValue);

		// Compute gradient for the given grid point {i,j,k},
		// tableOffset can be used [between 0 to 255]
		Vec<double, 3> ComputeRandomGradient(int i, int j, int k, const PermutationTable *pPermutationTable, int tableOffsetForSeed);

		/*****************************************************************************************************************
		* noise function implementation
		*****************************************************************************************************************/
		double octavePerlin3D(double x, double y, double z, const PerlinNoiseParams *pInParams)
		{
			double total = 0.0F;
			double maxValue = 0.0;
			double amplitude = 1.0;
			double frequency = 1.0;
			for (int i = 0; i < pInParams->octaves; i++)
			{
				total += perlin3D(x * frequency, y * frequency, z * frequency, pInParams) * amplitude;
				maxValue += amplitude;

				amplitude *= pInParams->persistence;
				frequency *= 2;
			}
			double normalized_total = total/maxValue;
			return normalized_total;
		}

		double perlin3D(double x, double y, double z, const PerlinNoiseParams *pInParams)
		{
			Vec<double, 3> point{ x, y, z };

			int i_x = floor(x);
			int i_y = floor(y);
			int i_z = floor(z);
			int i_x_nxt = i_x + 1;
			int i_y_nxt = i_y + 1;
			int i_z_nxt = i_z + 1;
			double u = x - i_x;
			double v = y - i_y;
			double w = z - i_z;
			
			// Nearest grid points
			// index zero represents element with zero offset, one with one offset
			Vec<double, 3> grid_points[2][2][2];
			grid_points[0][0][0] = Vec<double, 3>(i_x, i_y, i_z);
			grid_points[0][0][1] = Vec<double, 3>(i_x, i_y, i_z_nxt);
			grid_points[0][1][0] = Vec<double, 3>(i_x, i_y_nxt, i_z);
			grid_points[0][1][1] = Vec<double, 3>(i_x, i_y_nxt, i_z_nxt);
			grid_points[1][0][0] = Vec<double, 3>(i_x_nxt, i_y, i_z);
			grid_points[1][0][1] = Vec<double, 3>(i_x_nxt, i_y, i_z_nxt);
			grid_points[1][1][0] = Vec<double, 3>(i_x_nxt, i_y_nxt, i_z);
			grid_points[1][1][1] = Vec<double, 3>(i_x_nxt, i_y_nxt, i_z_nxt);

			// Directions from grid points to the given point
			Vec<double, 3> direction_from_grid_points[2][2][2];
			for (int i = 0; i < 1; i++)
			{
				for (int j = 0; j < 1; j++)
				{
					for (int k = 0; k < 1; k++)
					{
						direction_from_grid_points[i][j][k] = point - grid_points[i][j][k];
					}
				}
			}


			// Calculate gradients
			Vec<double, 3> gradients[2][2][2];
			for (int i = 0; i < 1; i++)
			{
				for (int j = 0; j < 1; j++)
				{
					for (int k = 0; k < 1; k++)
					{
						gradients[i][j][k] = ComputeRandomGradient(i_x + i, i_y + j, i_z + k, &(pInParams->permutationTable), pInParams->permutationTableOffsetForSeed);
					}
				}
			}

			double influence_values[2][2][2];
			for (int i = 0; i < 1; i++)
			{
				for (int j = 0; j < 1; j++)
				{
					for (int k = 0; k < 1; k++)
					{
						influence_values[i][j][k] = Dot(direction_from_grid_points[i][j][k], gradients[i][j][k]);
					}
				}
			}

			double combined_value = ComputeCombinedInfluenceValue(influence_values, u, v, w);
			return combined_value;
		}

		double ComputeCombinedInfluenceValue(double influence_values[2][2][2], double u, float v, double w)
		{
			double value_for_Z_ONE = ComputeCombinedInfluenceValue_ForGivenZ(influence_values, 1, u, v);
			double value_for_Z_ZERO = ComputeCombinedInfluenceValue_ForGivenZ(influence_values, 0, u, v);
			double interpolated_value = InterpolateInfluenceValues(value_for_Z_ZERO, value_for_Z_ONE, w);
			return interpolated_value;
		}

		double ComputeCombinedInfluenceValue_ForGivenZ(double influence_values[2][2][2], int InZ, double u, float v)
		{
			double value_for_Y_ONE = InterpolateInfluenceValues(influence_values[0][1][InZ], influence_values[1][1][InZ], u);
			double value_for_Y_ZERO = InterpolateInfluenceValues(influence_values[0][0][InZ], influence_values[1][0][InZ], u);
			double interpolated_value = InterpolateInfluenceValues(value_for_Y_ZERO, value_for_Y_ONE, v);
			return interpolated_value;
		}

		double InterpolateInfluenceValues(double InA, double InB, double w)
		{
			double w_pow_3 = w * w * w;
			double w_pow_4 = w_pow_3 * w;
			double w_pow_5 = w_pow_4 * w;
			double real_coeff = 6 * w_pow_5 - 15 * w_pow_4 + 10 * w_pow_3;
			double result = InA + (InB - InA) * w;
			return result;
		}

		void OutputResultGradient(Vec<double, 3> *pOutGradient, int InValue)
		{
			assert(pOutGradient);
			auto index = InValue % 12;
			switch (index)
			{
			case 0:
				*pOutGradient = Vec<double, 3>(1.0, 1.0, 0);
				return;

			case 1:
				*pOutGradient = Vec<double, 3>(-1.0, 1.0, 0);
				return;

			case 2:
				*pOutGradient = Vec<double, 3>(1.0, -1.0, 0);
				return;

			case 3:
				*pOutGradient = Vec<double, 3>(-1.0, -1.0, 0);
				return;

			case 4:
				*pOutGradient = Vec<double, 3>(1.0, 0.0, 1.0);
				return;

			case 5:
				*pOutGradient = Vec<double, 3>(-1.0, 0.0, 1.0);
				return;

			case 6:
				*pOutGradient = Vec<double, 3>(1.0, 0.0, -1.0);
				return;

			case 7:
				*pOutGradient = Vec<double, 3>(-1.0, 0.0, -1.0);
				return;

			case 8:
				*pOutGradient = Vec<double, 3>(0.0, 1.0, 1.0);
				return;

			case 9:
				*pOutGradient = Vec<double, 3>(0.0, -1.0, 1.0);
				return;

			case 10:
				*pOutGradient = Vec<double, 3>(0.0, 1.0, -1.0);
				return;

			case 11:
				*pOutGradient = Vec<double, 3>(0.0, -1.0, -1.0);
				return;

			default:
				assert(!"Shoult not get here");
			}
		}

		Vec<double, 3> ComputeRandomGradient(int i, int j, int k, const PermutationTable *pPermutationTable, int tableOffsetForSeed)
		{

			auto permutationTableIndex_X = pPermutationTable->GetAt(i + tableOffsetForSeed);
			auto permutationTableIndex_Y = pPermutationTable->GetAt(permutationTableIndex_X + j);
			auto permutationTableIndex_Z = pPermutationTable->GetAt(permutationTableIndex_Y + k);
			Vec<double, 3> gradient;
			OutputResultGradient(&gradient, permutationTableIndex_Z);
			return gradient;
		}

		/*****************************************************************************************************************
		* PermutationTable implementation
		*****************************************************************************************************************/
		PermutationTable::PermutationTable() :
			_table
			{
				151, 160, 137, 91, 90, 15,
				131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
				190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
				88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
				77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
				102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
				135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
				5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
				223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
				129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
				251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
				49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
				138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
			}
		{
		}
	}
}