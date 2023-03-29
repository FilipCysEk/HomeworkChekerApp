//
// Created by krzysiu on 02.11.2019.
//

#ifndef LIBRARIES1_CSVLIB_H
#define LIBRARIES1_CSVLIB_H


#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <map>
#include <iostream>

using namespace std;


class CSVlib {
public:
    /**
     * @brief Create object and open file and read it
     * @param fileName csv file name
     * @param head csv file have first line with col titles
     * @param delimiter default ','
     */
    CSVlib(string fileName, bool head = true, char delimiter = ',');

    CSVlib() {};

    /**
     * @brief Open csv file and read it
     * @param fileName csv file name
     * @param head csv file have first line with col titles
     * @param delimiter default ','
     * @return true if success or false if error
     */
    bool open(string filename, bool head = true, char delimiter = ',');

    /**
     * @brief Check critical error
     * @return
     */
    bool isOk();

    /**
     * @brief Return head. If enabled head, return head, if not generated
     * @return
     */
    vector<string> getHead();

    /**
     * @brief Return all records
     * @return
     */
    vector<vector<string>> getAll();

    /**
     * @brief Returns pointer to all records
     * @return
     */
    vector<vector<string>> *getAllPointer();

    /**
     * @brief Return one row
     * @return
     */
    //map<string, string> getRow();
    vector<string> getRow();

    /**
     * @brief Return pointer to one row
     * @return
     */
    vector<string> *getPointerRow();

    /**
     * @brief Return one row and increment iterator
     * @return
     */
    vector<string> getRowNext();

    /**
     * @brief Return pointer to one row and increment iterator
     * @return
     */
    vector<string> *getPointerRowNext();

    /**
     * @brief Return vector with all rows
     * @return
     */
    map<string, vector<string>> getAllMap();

    /**
     * @brief Return one row
     * @return
     */
    //map<string, string> getRow();
    map<string, string> getRowMap();

    /**
     * @brief Return one row and increment iterator
     * @return
     */
    map<string, string> getRowNextMap();

    /**
     * @brief Increment iterator
     * @return if has next true, if not have false
     */
    bool next();

    /**
     *
     * @return amount of rows
     */
    int size();

    /**
     * @brief Set iterator to begin (0)
     */
    void setIteratorOnBegin();

    /**
     * @breif Ignoring white chars on begin and end field
     * @param ignoreWhiteChars true - yes, false - no; default false
     */
    void setIgnoreWhiteChars(bool ignoreWhiteChars);

    /**
     * @brief Returns amount lines where is error
     * @return
     */
    unsigned short getReadLinesError();

    /**
     * @brief Returns vectors with all errors
     * @return
     */
    vector<string> getErrorsList();

private:
    ifstream csvFile;
    vector<vector<string>> rows;
    vector<string> headV;
    vector<string> errors;
    bool criticalError = false;
    char delimiter = ',';
    bool head = true;
    bool ignoreWhiteChars = false;
    unsigned short readLinesError = 0;
    unsigned int iter = 0;

    void read();

    string prepareString(string txt);

    string deleteWhiteChars(string txt);

    string correctQuotes(string txt);

    int count(string txt, string find);

    void addError(string msg, bool critical = false);

    vector<string> getFileRow(string line);
};


#endif //LIBRARIES1_CSVLIB_H
