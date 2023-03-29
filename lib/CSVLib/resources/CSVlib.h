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

using namespace std;


class CSVlib {
public:
    /**
     * @brief Create object and open file and read it
     * @param fileName csv file name
     * @param delimiter default ','
     */
    CSVlib(string fileName, char delimiter = ',');
    CSVlib(){};

    /**
     * @brief Open csv file and read it
     * @param fileName csv file name
     * @param delimiter default ','
     * @return true if success or false if error
     */
    bool open(string filename, char delimiter = ',');

    /**
     * @brief Check that csv was readed (readed rows > 0)
     * @return
     */
    bool isReaded();

    /**
     * @brief Return vector with all rows
     * @return
     */
    vector<map<string, string>> getAll();

    /**
     * @brief Return pointer to vector with all rows
     * @return
     */
    vector<map<string, string>>* getAllPointer();

    /**
     * @brief Return one row
     * @return
     */
    map<string, string> getRow();

    /**
     * @brief Return pointer to one row
     * @return
     */
    map<string, string>* getPointerRow();

    /**
     * @brief Return one row and increment iterator
     * @return
     */
    map<string, string> getRowNext();

    /**
     * @brief Return pointer to one row and increment iterator
     * @return
     */
    map<string, string>* getPointerRowNext();

    /**
     * @brief Increment iterator
     */
    void next();

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

private:
    ifstream csvFile;
    vector<map<string, string>> rows;
    char delimiter = ',';
    bool ignoreWhiteChars = false;
    unsigned short readLinesError = 0;
public:
    unsigned short getReadLinesError() const;

private:
    unsigned int iter = 0;

    void read();
    string prepareString(string txt);
    string deleteWhiteChars(string txt);
    string correctQuotes(string txt);
    int count(string txt, string find);
};


#endif //LIBRARIES1_CSVLIB_H
