//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Feb  9 21:05:41 PST 2018
// Last Modified: Fri Feb  9 21:05:45 PST 2018
// Filename:      tool-chord.cpp
// URL:           https://github.com/craigsapp/humlib/blob/master/src/tool-chord.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   Interface for fixing adjusting chords (order of pitches,
//                rhythms, articulatios).
//

#include "tool-chord.h"
#include "Convert.h"
#include "HumRegex.h"


using namespace std;

namespace hum {

// START_MERGE


/////////////////////////////////
//
// Tool_chord::Tool_chord -- Set the recognized options for the tool.
//

Tool_chord::Tool_chord(void) {
	// add options here
	define("u|sort-upwards=b",    "sort notes by lowest first in chord");
	define("d|sort-downwards=b",  "sort notes by highest first in chord");
	define("t|top-note=b",        "extract top note of chords");
	define("b|bottom-note=b",     "extract bottom note of chords");
	define("f|first-note=b",      "extract first note of chords");
	define("l|last-note=b",       "extract last note of chords");
	define("s|spine=i:-1",        "spine to process (indexed from 1)");
}



/////////////////////////////////
//
// Tool_chord::run -- Do the main work of the tool.
//

bool Tool_chord::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_chord::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_chord::run(HumdrumFile& infile) {
	initialize();
	processFile(infile, m_direction);
	infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_chord::initialize --
//

void Tool_chord::initialize(void) {
	m_direction = 1;
	if (getBoolean("sort-upwards")) {
		m_direction = -1;
	}
	if (getBoolean("sort-downwards")) {
		m_direction = +1;
	}
	m_spine = getInteger("spine");
}



//////////////////////////////
//
// Tool_chord::processFile --
//     direction:  1 = highest first
//     direction: -1 = lowest first
//

void Tool_chord::processFile(HumdrumFile& infile, int direction) {
	if (!(getBoolean("top-note") || getBoolean("bottom-note") ||
			getBoolean("sort-upwards") || getBoolean("sort-downwards") ||
			getBoolean("first-note") || getBoolean("last-note"))) {
		// nothing to do
		return;
	}

	HumRegex hre;
	for (int i=0; i<infile.getStrandCount(); i++) {
		HTp stok = infile.getStrandStart(i);
		int track = stok->getTrack();
		if ((m_spine > 0) && (track != m_spine)) {	
			continue;
		}
		if (!stok->isKern()) {
			continue;
		}
		HTp etok = infile.getStrandEnd(i);
		HTp tok = stok;
		while (tok && (tok != etok)) {
			if (!tok->isData()) {
				tok = tok->getNextToken();
				continue;
			}
			if (tok->isNull()) {
				tok = tok->getNextToken();
				continue;
			}
			if (!tok->isChord()) {
				tok = tok->getNextToken();
				continue;
			}
			processChord(tok, direction);
			tok = tok->getNextToken();
		}
	}
}



//////////////////////////////
//
// Tool_chord::processChord --
//

void Tool_chord::processChord(HTp tok, int direction) {
	vector<string> notes;
	int count = (int)tok->getSubtokenCount();
	for (int i=0; i<count; i++) {
		notes.emplace_back(tok->getSubtoken(i));
	}

	if (notes.size() <= 1) {
		// nothing to do
		return;
	}

	vector<pair<int, int>> pitches(count);
	for (int i=0; i<(int)pitches.size(); i++) {
		pitches[i].first = Convert::kernToBase40(notes[i]);
		pitches[i].second = i;
	}

	if (getBoolean("top-note")) {
		direction = -1;
	}
	if (getBoolean("bottom-note")) {
		direction = -1;
	}
	if (getBoolean("first-note")) {
		direction = 0;
	}
	if (getBoolean("last-note")) {
		direction = 0;
	}

	if (direction > 0) {
		sort(pitches.begin(), pitches.end(), 
			[](const pair<int, int>& a, const pair<int, int>& b) -> bool {
				return a.first > b.first;
			});
	} else if (direction < 0) {
		sort(pitches.begin(), pitches.end(), 
			[](const pair<int, int>& a, const pair<int, int>& b) -> bool {
				return a.first < b.first;
			});
	}

	bool same = true;
	for (int i=1; i<(int)pitches.size(); i++) {
		if (pitches[i].second != pitches[i-1].second + 1) {
			same = false;
			break;
		}
	}
	if ((getBoolean("sort-upwards") || getBoolean("sort-downwards")) && same) {
		return;
	}

	string prefix;
	string suffix;

	HumRegex hre;
	if (hre.search(notes[0], "(&*\\([<>]?)")) {
		prefix = hre.getMatch(1);
		hre.replaceDestructive(notes[0], "", "(&*\\([<>]?)");
	}

	string beam;
	int beamindex = -1;
	for (int i=0; i<(int)notes.size(); i++) {
		if (hre.search(notes[i], "([LJkK]+[<>]?)")) {
			beamindex = i;
			beam = hre.getMatch(1);
			hre.replaceDestructive(notes[i], "", "([LJkK]+[<>]?)");
		}
	}
	
	if (hre.search(notes.back(), "(&*\\)[<>]?)")) {
		suffix += hre.getMatch(1);
		hre.replaceDestructive(notes.back(), "", "(&*\\)[<>]?)");
	}

	string output = prefix;
	if (getBoolean("top-note")) {
		output += notes[pitches.back().second];
	} else if (getBoolean("bottom-note")) {
		output += notes[pitches[0].second];
	} else if (getBoolean("first-note")) {
		output += notes[pitches[0].second];
	} else if (getBoolean("last-note")) {
		output += notes[pitches.back().second];
	} else {
		for (int i=0; i<(int)pitches.size(); i++) {
			output += notes[pitches[i].second];
			if (beamindex == i) {
				output += beam;
			}
			if (i < (int)pitches.size() - 1) {
				output += " ";
			}
		}
	}

	output += suffix;
	tok->setText(output);
}


// END_MERGE

} // end namespace hum



