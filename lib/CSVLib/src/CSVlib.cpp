//
// Created by krzysiu on 02.11.2019.
//

#include <sstream>
#include "../include/CSVlib.h"

CSVlib::CSVlib(string fileName, bool head, char delimiter)
        : delimiter(delimiter), head(head) {
    open(fileName, head, delimiter);
}

bool CSVlib::open(string filename, bool head, char delimiter) {
    this->delimiter = delimiter;
    this->head = head;
    csvFile.open(filename);
    if (csvFile.is_open()) {
        read();
        csvFile.close();
    } else {
        addError("Can't open file: " + filename, true);
        return false;
    }
}

bool CSVlib::isOk() {
    return !criticalError;
}

void CSVlib::read() {
    //Get col names
    string rowtxt;
    if (head) {
        if (getline(csvFile, rowtxt)) {
            headV = getFileRow(rowtxt);

            //Checking strings and deleting quotes
            for (string &cell: headV) {
                if (cell.size() != 0) {
                    cell = prepareString(cell);
                }
            }
        } else {
            addError("Can't get line when getting head", true);
            return;
        }
    }

    //Getting rest
    while (getline(csvFile, rowtxt)) {
        if (isspace(rowtxt[rowtxt.size() - 1]))
            rowtxt.pop_back();

        rows.push_back(getFileRow(rowtxt));
    }

    //Checking correction (all rows has the same cells)
    int cols;
    if (head) {
        cols = headV.size();
    } else {
        cols = rows[0].size();
    }

    int lineNum = 1;
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i].size() != cols) {
            addError("Line " + to_string(lineNum) + " have no enought cells");
            rows.erase(rows.begin() + i);
            //Save to not skip some rows
            i--;
            lineNum++;
            continue;
        }

        //Checking strings and deleting quotes
        for (string &cell: rows[i]) {
            if (cell.size() != 0) {
                cell = prepareString(cell);
            }
        }
        lineNum++;
    }

    //Creating head if not created
    if (!head) {
        for (int i = 0; i < rows[0].size(); i++) {
            headV.push_back(to_string(i));
        }
    }

}

vector<string> CSVlib::getFileRow(string line) {
    vector<string> row;
    stringstream lineStream(line);
    string field;
    //Read row
    while (getline(lineStream, field, delimiter)) {
        row.push_back(field);
    }

    //Check that last sign is delimiter
    if (line[line.size() - 1] == delimiter)
        row.emplace_back("");

    return row;
}

string CSVlib::prepareString(string txt) {
    short cquotes = count(txt, "\"");
    //cout << txt << " - " << cquotes << " - " << (cquotes % 2 != 0) << endl;
    if (cquotes % 2 != 0) {
        addError("Not even amount of quotes");
        return "";
    }

    if (cquotes == 0) {
        //If not find quotes
        if (ignoreWhiteChars) {
            return deleteWhiteChars(txt);
        } else {
            return txt;
        }
    } else {
        //If find quotes
        short doublecquotes = count(txt, "\"\"");
        if (cquotes == doublecquotes + 2) {
            //If find single qoutes
            string txtout = deleteWhiteChars(txt);
            //Checking quotes on begin and end
            //cout<< txtout << " " << (int)txtout[0] << "-" << txtout[0] <<endl;
            /*if(txtout == "\"\""){
                txtout = "";
            } else*/
            if (txtout[0] == '\"' && txtout[txtout.size() - 1] == '\"') {
                txtout = txtout.substr(1, txtout.size() - 2);
                return correctQuotes(txtout);
            } else {
                //throeing error
                addError("Find one quote in: " + txt);
                return "";
            }
        } else {
            //If only double quotes
            if (ignoreWhiteChars) {
                txt = deleteWhiteChars(txt);
            }

            return correctQuotes(txt);
        }
    }
}

void CSVlib::setIgnoreWhiteChars(bool ignoreWhiteChars) {
    this->ignoreWhiteChars = ignoreWhiteChars;
}

string CSVlib::deleteWhiteChars(string txt) {
    //Delete white chars from begin
    int indx = 0;
    while (isspace(txt[indx]) && indx < txt.size()) {
        indx++;
    }
    txt = txt.substr(indx);
    indx++;

    //delete white chars from end
    indx = txt.size() - 1;
    while (indx > 0 && isspace(txt[indx])) {
        indx--;
    }
    txt = txt.substr(0, indx + 1);
    return txt;
}

string CSVlib::correctQuotes(string txt) {
    //corecting quotes and checking correction
    if (txt == "\"\"")
        return "";

    int i = 0;
    while (i < txt.size()) {
        if (txt[i] == '"') {
            if (txt[i + 1] == '"') {
                txt.erase(i, 1);
            } else {
                addError("Quotes error in line: " + txt);
                return "";
            }
        }
        i++;
    }
    return txt;
}

void CSVlib::setIteratorOnBegin() {
    iter = 0;
}

bool CSVlib::next() {
    if (iter < rows.size() - 1) {
        iter++;
        return true;
    } else {
        return false;
    }

}

int CSVlib::count(string txt, string find) {
    if (txt.size() == 0 || find.size() == 0)
        return 0;

    int count = 0;
    for (int i = 0; i <= txt.size() - find.size(); i++) {
        if (txt[i] == find[0]) {
            bool equal = true;
            int j;
            for (j = 0; j < find.size(); j++) {
                if (txt[i + j] != find[j]) {
                    equal = false;
                    break;
                }
            }
            i += j - 1;
            if (equal) count++;
        }
    }

    return count;
}

unsigned short CSVlib::getReadLinesError() {
    return readLinesError;
}

void CSVlib::addError(string msg, bool critical) {
    errors.push_back(msg);
    readLinesError++;
    criticalError = critical;
}

vector<string> CSVlib::getErrorsList() {
    return errors;
}

int CSVlib::size() {
    return rows.size();
}

map<string, vector<string>> CSVlib::getAllMap() {
    map<string, vector<string>> output;
    short sizeHead = headV.size();

    for (string &heads : headV) {
        output[heads] = vector<string>();
    }

    for (auto &row :rows) {
        for (int i = 0; i < sizeHead; i++) {
            output[headV[i]].push_back(row[i]);
        }
    }
    return output;
}

vector<string> CSVlib::getHead() {
    return headV;
}

vector<vector<string>> CSVlib::getAll() {
    return rows;
}

vector<vector<string>> *CSVlib::getAllPointer() {
    return &rows;
}

vector<string> CSVlib::getRow() {
    return rows[iter];
}

vector<string> *CSVlib::getPointerRow() {
    return &rows[iter];
}

vector<string> CSVlib::getRowNext() {
    if (next())
        return rows[iter - 1];
    else
        return rows[iter];
}

vector<string> *CSVlib::getPointerRowNext() {
    if (next())
        return &rows[iter - 1];
    else
        return &rows[iter];
}

map<string, string> CSVlib::getRowMap() {
    map<string, string> output;
    for (int i = 0; i < headV.size(); i++) {
        output[headV[i]] = (rows[iter][i]);
    }
    return output;
}

map<string, string> CSVlib::getRowNextMap() {
    map<string, string> output;
    if (next()) {
        for (int i = 0; i < headV.size(); i++) {
            output[headV[i]] = (rows[iter - 1][i]);
        }
    } else {
        for (int i = 0; i < headV.size(); i++) {
            output[headV[i]] = (rows[iter][i]);
        }
    }

    return output;
}


