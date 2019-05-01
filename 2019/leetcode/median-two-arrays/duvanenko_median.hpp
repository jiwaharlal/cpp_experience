// Listing One

// The median check consists of two checks:
// 1. Is the element to the right of this element equal to or larger
// 2. Is the element to the left  of this element equal to or smaller
// Returns a boolean to indicate whether the median was found in array A.
template< class _Type >
inline boolean medianOfTwoSortedArrays_m2_inner( const _Type* a, long a_left, long a_right, const _Type* b, long b_left, long b_right,
												 long num_elements_larger_than_median_in_total, long* median )
{
	long low   = a_left;
	long high  = __max( a_left, a_right );
	while( low <= high )					// termination means no overall median within A
	{
		long overall_median           = ( low + high ) / 2;		// guess for the median as middle of A
		long num_larger_elements_in_a = a_right - overall_median;
		long num_larger_elements_in_b = num_elements_larger_than_median_in_total - num_larger_elements_in_a;
		long i_larger_in_b            = b_right - num_larger_elements_in_b + 1;	// starting index in array B of elements larger than median of A
		// Compare with the left element in B if there is one
		if ( i_larger_in_b <= b_left )				// no elements to the left in B. #2 check can't be performed
		{
			if ( a[ overall_median ] <= b[ b_left ] && i_larger_in_b == b_left )		// satisfies being an overall median. check the element to the right (check #1)
			{
				*median = overall_median;
				return true;						// median was found in A
			}
			else {	// a[ overall_median ] > b[ i_larger_in_b = 0 ] => guess of overal median is too big to be the overall median. Need to move overall_median within A to become smaller
				high = overall_median - 1;			// do not include it in our further searches, since it's not the overall median
			}
		}	// i_larger_in_b > 0  => have a left element to compare with. Compare with the right element in B if there is one
		else if ( i_larger_in_b > b_right )			// no elements to the right in B. #1 check can't be performed
		{
			if ( b[ b_right ] <= a[ overall_median ] && ( i_larger_in_b - 1 ) == b_right )	// satisfies being an overall median
			{
				*median = overall_median;
				return true;						// median was found in A
			}
			else {	// a[ overall_median ] < b[ i_larger_in_b = b_right ] => guess of overall median is too small to be the overall median. Need to move overall_median within A to become smaller
				low = overall_median + 1;			// do not iclude it in our further searches, since it's not the overall median
			}
		}	// 0 < i_larger_in_b <= b_right => have two elements within B to compare with a[ overall_median ]
		else if ( b[ i_larger_in_b - 1 ] <= a[ overall_median ])	// check the element to the left  (check #2)
		{
			if ( a[ overall_median ] <= b[ i_larger_in_b ])			// check the element to the right (check #1)
			{
				*median = overall_median;
				return true;						// median was found in A
			}
			else {
				high = overall_median - 1;	// do not include it in our further searches, since it's not the overall median
			}
		}
		else {	// a[ overall_median ] < b[ i_larger_in_b - 1 ] => overall median guess is too small and needs to be increased
			low = overall_median + 1;		// do not iclude it in our further searches, since it's not the overall median
		}
	}
	*median = -1;
	return false;	// median was not found in A
}
// Searches both arrays, since the median can be in either array no matter the size of either array. Even if one of the arrays has a single element, that element could be the median.
// Both array bounds (left and right) are inclusive.
// Starts by searching within the first array and if the median is not found within the first array, then the search continues within the second array.
template< class _Type >
inline void medianOfTwoSortedArrays_m2( const _Type* a, long a_left, long a_right, const _Type* b, long b_left, long b_right, long* which_array, long* median )
{
	*which_array =  0;
	*median      = -1;		// by default set return median index to be invalid
	long a_length = a_right - a_left + 1;
	long b_length = b_right - b_left + 1;
	long total_length    = a_length + b_length;
	long median_in_total = ( total_length - 1 ) / 2;	// works for even or odd number of elements
	long num_elements_larger_than_median_in_total = ( total_length - 1 ) - median_in_total;		// high element in total minus median index
	// There may be a way to eliminate this special case of handling either one or both input arrays of length 0
	if ( a_length == 0 )
	{
		if ( b_length == 0 ) {
			*which_array =  0;			// both arrays are of zero length, return an invalid overall median index
			*median      = -1;
		} else {	// b_length != 0
			*which_array = 1;			// median was found in B immediately since A has no elements
			*median      = b_left + median_in_total;
		}
		return;
	}
	else if ( b_length == 0 ) {
		*which_array = 0;				// median was found in A immediately since B has no elements
		*median      = a_left + median_in_total;
		return;
	}
	// a_length > 0 and b_length > 0
	if ( medianOfTwoSortedArrays_m2_inner( a, a_left, a_right, b, b_left, b_right, num_elements_larger_than_median_in_total, median ))
	{
		*which_array = 0;
		return;
	}
	// Search within B, since we didn't find it within A. It has to exist within B, since there is always an overall median.
	if ( medianOfTwoSortedArrays_m2_inner( b, b_left, b_right, a, a_left, a_right, num_elements_larger_than_median_in_total, median ))
	{
		*which_array = 1;
		return;
	}
	*which_array = -1;
	*median      = -2;	// There has to be an overall median at this point, flag an error condition if it wasn't found.
	return;
}
