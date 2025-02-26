using namespace std;

int int_countDigits(int n)
{
    int count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}

vector<int> int_splitDigits(int n) {
    int sum = 0;
    vector<int> result;
    int digits = int_countDigits(n);
    for (int i = digits; i > 0; i--)
    {
        result[i - 1] = (n % 10);
        n = (n / 10);
        sum += result[i - 1];
    }

    return result;
}