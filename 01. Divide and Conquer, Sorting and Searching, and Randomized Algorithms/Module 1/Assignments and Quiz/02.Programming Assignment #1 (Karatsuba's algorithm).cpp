/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 15th Of FEBURARY, 2023
 *
 * Description: In this programming assignment you will implement Karatsuba's algorithm described in lecture.
 *
 * Dependencies: This Code requires a txt file named "02.int_values_PA.txt" which contains both the numbers which to be multiplied. (you can change the numbers too)
 */

#include <iostream>
#include <fstream> //for ifstream
#include <string>
#include <algorithm> //for reverse()


using namespace std;

string integerMultiplication (string x, string y) {
    //because its multiplication of only one-digit numbers, we can use int
    int x_int = stoi(x);
    int y_int = stoi(y);
    string result;
    result = to_string(x_int * y_int);
    return result;
}

//pass by reference so we can make changes instantly and not copy the string
void putZero(string* num, int n) {
    //We cant convert the string into integer, because number might be way too big
    for(int i = 0 ; i < n; i++) {
        *num += "0";
    }
}

string combine(string str1, string str2) {
    //Again, We cant convert the string into integer, because number might be way too big

    //for result:
    string result;

    //We make sure str2 is longer
    if(str1.size() > str2.size()) {
        swap(str1, str2);
    }

    int n1 = str1.size();
    int n2 = str2.size();

    int carry = 0;

    int j, i;
    j = n2 - 1;
    for(i = n1-1; i >= 0; i--) {

        int sum = ((str1[i]-'0')+(str2[j]-'0')+carry);
        result.push_back(sum%10 + '0');
        j--;
        carry = sum/10;
    }
    for(; j >= 0; j--) {

        int sum = ((str2[j]-'0')+carry);
        result.push_back(sum%10 + '0');
        carry = sum/10;
    }

    if (carry != 0){
        result.push_back(carry+'0');
    }

    reverse(result.begin(), result.end());

    //cout << str1 << " + " << str2 << " = " << result << endl;

    return result;
}


string karatsuba (string x, string y, int n) {
    if (n != 1) {

        string a = x.substr(0, n/2);
        string b = x.substr(n/2);
        string c = y.substr(0, n/2);
        string d = y.substr(n/2);

        string ac = karatsuba(a, c, n/2);
        string bd = karatsuba(b, d, n/2);
        string ad = karatsuba(a, d, n/2);
        string bc = karatsuba(b, c, n/2);

        string ad_plus_bc = combine(ad, bc);

        //cout << "\nis: " << ad_plus_bc << "\n";
        putZero(&ac, n);
        putZero(&ad_plus_bc, n/2);

/*
        cout << "ac * 10^n: " << ac << "\n";
        cout << "bd: " << bd << "\n";
        cout << "ad: " << ad << "\n";
        cout << "bc: " << bc << "\n";
        cout << "ad+bc * 10^n/2: " << ad_plus_bc << "\n";
*/

        string result = combine(combine(ac, ad_plus_bc), bd);
        return result;

    }
    else {
        //cout << "x: " << x << " y: " << y << "\t" << "x.y: " << integerMultiplication(x, y) << "\n";
        return integerMultiplication(x, y);
    }
}
int main () {
    ifstream myfile("02.int_values_PA.txt"); //You can change the name of the file if you please.
    if(!myfile.is_open())
    {
        cout << "File failed to open" << endl;
        return 0;
    }

    string x,y;
    myfile >> x >> y;
    cout << "x \t= " << x << endl << "y \t= " << y << endl;
    cout << endl;


    cout << "Karatsuba Multiplication : " << karatsuba(x, y, x.size()) << endl;

    //cout << combine(x, y) << endl;
}
