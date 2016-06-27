//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Aug 17 02:39:28 PDT 2015
// Last Modified: Mon Aug 17 02:39:32 PDT 2015
// Filename:      HumdrumFileContent.h
// URL:           https://github.com/craigsapp/humlib/blob/master/include/HumdrumFileContent.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   Used to add content analysis to HumdrumFileStructure class.
//

#ifndef _HUMDRUMFILECONTENT_H
#define _HUMDRUMFILECONTENT_H

#include "HumdrumFileStructure.h"

using namespace std;

namespace hum {

// START_MERGE

class HumdrumFileContent : public HumdrumFileStructure {
	public:
		       HumdrumFileContent         (void);
		       HumdrumFileContent         (const string& filename);
		       HumdrumFileContent         (istream& contents);
		      ~HumdrumFileContent         ();

		bool   analyzeKernSlurs           (void);
		bool   analyzeKernTies            (void);
		bool   analyzeKernAccidentals     (void);

	protected:
		bool   analyzeKernSlurs           (HumdrumToken* spinestart);
		bool   analyzeKernTies            (HumdrumToken* spinestart);
		void   fillKeySignature           (vector<int>& states,
		                                   const string& keysig);
		void   resetDiatonicStatesWithKeySignature(vector<int>& states,
				                             vector<int>& signature);
};


// END_MERGE

} // end namespace hum

#endif /* _HUMDRUMFILECONTENT_H */


