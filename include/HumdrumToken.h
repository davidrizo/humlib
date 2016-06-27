//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  8 12:24:49 PDT 2015
// Last Modified: Sun Aug  9 21:03:12 PDT 2015
// Filename:      HumdrumToken.h
// URL:           https://github.com/craigsapp/humlib/blob/master/include/HumdrumToken.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   Used to work with Humdrum fields.
//

#ifndef _HUMDRUM_TOKEN_H
#define _HUMDRUM_TOKEN_H

#include <iostream>
#include <vector>

#include "HumNum.h"
#include "HumAddress.h"
#include "HumHash.h"

using namespace std;

namespace hum {

// START_MERGE


class HumdrumToken : public string, public HumHash {
	public:
		         HumdrumToken              (void);
		         HumdrumToken              (const char* token);
		         HumdrumToken              (const string& token);
		        ~HumdrumToken              ();

		bool     isNull                    (void) const;
		bool     isManipulator             (void) const;

		bool     isExclusiveInterpretation (void) const;
		bool     isSplitInterpretation     (void) const;
		bool     isMergeInterpretation     (void) const;
		bool     isExchangeInterpretation  (void) const;
		bool     isTerminateInterpretation (void) const;
		bool     isAddInterpretation       (void) const;

		// alises for the above
		bool     isExclusive     (void) const {
		                                   return isExclusiveInterpretation(); }
		bool     isExInterp      (void) const {
		                                   return isExclusiveInterpretation(); }
		bool     isSplit         (void) const { return isSplitInterpretation(); }
		bool     isMerge         (void) const { return isMergeInterpretation(); }
		bool     isExchange      (void) const {
		                                   return isExchangeInterpretation(); }
		bool     isTerminate     (void) const {
		                                   return isTerminateInterpretation(); }
		bool     isTerminator    (void) const {
		                                   return isTerminateInterpretation(); }
		bool     isAdd           (void) const { return isSplitInterpretation(); }

		bool     isBarline                 (void) const;
		bool     isCommentLocal            (void) const;
		bool     isComment                 (void) const;
		bool     isData                    (void) const;
		bool     isInterpretation          (void) const;
		bool     isNonNullData             (void) const;
		bool     isNullData                (void) const;
		bool     isChord                   (const string& separator = " ");
		bool     hasRhythm                 (void) const;

		// kern-specific functions:
		bool     isRest                    (void) const;
		bool     isNote                    (void) const;
		bool     isSecondaryTiedNote       (void) const;
		bool     isInvisible               (void) const;
		bool     isGrace                   (void) const;
		bool     isClef                    (void) const;

		bool     hasSlurStart              (void) const;
		bool     hasSlurEnd                (void) const;
		int      hasVisibleAccidental      (int subtokenIndex) const;
		int      hasCautionaryAccidental   (int subtokenIndex) const;

		HumNum   getDuration               (void) const;
		HumNum   getDuration               (HumNum scale) const;

		HumNum   getDurationFromStart      (void) const;
		HumNum   getDurationFromStart      (HumNum scale) const;

		HumNum   getDurationToEnd          (void) const;
		HumNum   getDurationToEnd          (HumNum scale) const;

		HumNum   getBarlineDuration        (void) const;
		HumNum   getBarlineDuration        (HumNum scale) const;

		HumdrumLine* getOwner              (void) const;
		HumdrumLine* getLine               (void) const { return getOwner(); }
		bool     equalChar                 (int index, char ch) const;

		int      getLineIndex              (void) const;
		int      getLineNumber             (void) const;
		int      getFieldIndex             (void) const;
		int      getTokenIndex (void) const { return getFieldIndex(); }
		const string& getDataType          (void) const;
		bool     isDataType                (string dtype) const;
		bool     isKern                    (void) const;
		string   getSpineInfo              (void) const;
		int      getTrack                  (void) const;
		int      getSubtrack               (void) const;
		string   getTrackString            (void) const;
		int      getSubtokenCount          (const string& separator = " ") const;
		string   getSubtoken               (int index,
		                                    const string& separator = " ") const;
		void     setParameters             (HumdrumToken* ptok);
		void     setParameters             (const string& pdata,
		                                    HumdrumToken* ptok = NULL);
		int      getStrandIndex            (void) const;
		int      getSlurStartElisionLevel  (void) const;
		int      getSlurEndElisionLevel    (void) const;
		ostream& printCsv                  (ostream& out = cout);
		ostream& printXml                  (ostream& out = cout, int level = 0,
		                                    const string& indent = "\t");
		string   getXmlId                  (const string& prefix = "") const;
		string   getXmlIdPrefix            (void) const;

		// next/previous token functions:
		int           getNextTokenCount         (void) const;
		int           getPreviousTokenCount     (void) const;
		HumdrumToken* getNextToken              (int index = 0) const;
		HumdrumToken* getPreviousToken          (int index = 0) const;
		vector<HumdrumToken*> getNextTokens     (void) const;
		vector<HumdrumToken*> getPreviousTokens (void) const;

		int      getPreviousNonNullDataTokenCount(void);
		int      getPreviousNNDTCount(void) {
		               return getPreviousNonNullDataTokenCount(); }
		HumdrumToken* getPreviousNonNullDataToken(int index = 0);
		HumdrumToken* getPreviousNNDT(int index) {
		               return getPreviousNonNullDataToken(index); }
		int      getNextNonNullDataTokenCount(void);
		int      getNextNNDTCount(void) { return getNextNonNullDataTokenCount(); }
		HumdrumToken* getNextNonNullDataToken(int index = 0);
		HumdrumToken* getNextNNDT(int index = 0) {
		               return getNextNonNullDataToken(index); }

		// slur-analysis based functions:
		HumNum   getSlurDuration   (HumNum scale = 1);

	protected:
		void     setLineIndex      (int lineindex);
		void     setFieldIndex     (int fieldlindex);
		void     setSpineInfo      (const string& spineinfo);
		void     setTrack          (int aTrack, int aSubtrack);
		void     setTrack          (int aTrack);
		void     setSubtrack       (int aSubtrack);
		void     setSubtrackCount  (int count);
		void     setPreviousToken  (HumdrumToken* aToken);
		void     setNextToken      (HumdrumToken* aToken);
		void     makeForwardLink   (HumdrumToken& nextToken);
		void     makeBackwardLink  (HumdrumToken& previousToken);
		void     setOwner          (HumdrumLine* aLine);
		int      getState          (void) const;
		void     incrementState    (void);
		void     setDuration       (const HumNum& dur);
		void     setStrandIndex    (int index);

		bool     analyzeDuration   (string& err);
		ostream& printXmlBaseInfo  (ostream& out = cout, int level = 0,
		                            const string& indent = "\t");
		ostream& printXmlContentInfo(ostream& out = cout, int level = 0,
		                            const string& indent = "\t");
		ostream& printXmlStructureInfo(ostream& out = cout, int level = 0,
		                            const string& indent = "\t");
		ostream& printXmlParameterInfo(ostream& out = cout, int level = 0,
		                            const string& indent = "\t");

	private:

		// address: The address contains information about the location of
		// the token on a HumdrumLine and in a HumdrumFile.
		HumAddress address;

		// duration: The duration of the token.  Non-rhythmic data types
		// will have a negative duration (which should be interpreted
		// as a zero duration--See HumdrumToken::hasRhythm()).
		// Grace note will have a zero duration, even if they have a duration
		// list in the token for a graphical display duration.
		HumNum duration;

		// nextTokens: This is a list of all previous tokens in the spine which
		// immediately precede this token. Typically there will be one
		// following token, but there can be two tokens if the current
		// token is *^, and there will be zero following tokens after a
		// spine terminating token (*-).
		vector<HumdrumToken*> nextTokens;     // link to next token(s) in spine

		// previousTokens: Simiar to nextTokens, but for the immediately
		// follow token(s) in the data.  Typically there will be one
		// preceding token, but there can be multiple tokens when the previous
		// line has *v merge tokens for the spine.  Exclusive interpretations
		// have no tokens preceding them.
		vector<HumdrumToken*> previousTokens; // link to last token(s) in spine

		// nextNonNullTokens: This is a list of non-tokens in the spine
		// that follow this one.
		vector<HumdrumToken*> nextNonNullTokens;

		// previousNonNullTokens: This is a list of non-tokens in the spine
		// that preced this one.
		vector<HumdrumToken*> previousNonNullTokens;

		// rhycheck: Used to perfrom HumdrumFileStructure::analyzeRhythm
		// recursively.
		int rhycheck;

		// strand: Used to keep track of contiguous voice connections between
      // secondary spines/tracks.  This is the 1-D strand index number
		// (not the 2-d one).
		int strand;

	friend class HumdrumLine;
	friend class HumdrumFileBase;
	friend class HumdrumFileStructure;
	friend class HumdrumFileContent;
	friend class HumdrumFile;
};


typedef HumdrumToken* HTp;

ostream& operator<<(ostream& out, const HumdrumToken& token);
ostream& operator<<(ostream& out, HumdrumToken* token);

ostream& printSequence(vector<vector<HTp> >& sequence, ostream& out=std::cout);
ostream& printSequence(vector<HTp>& sequence, ostream& out = std::cout);

// END_MERGE

} // end namespace hum

#endif /* _HUMDRUM_TOKEN_H */


