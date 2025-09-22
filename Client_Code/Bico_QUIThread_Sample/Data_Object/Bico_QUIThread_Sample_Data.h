#ifndef BICO_QUITHREAD_SAMPLE_DATA_H
#define BICO_QUITHREAD_SAMPLE_DATA_H

class Bico_QUIThread_Sample_Data
{
public:
    Bico_QUIThread_Sample_Data();
    

    int getData_1() const;
    void setData_1(int value);

    float getData_2() const;
    void setData_2(float value);

private:
    int data_1;
    float data_2;
};

#endif // BICO_QWINDOWTHREAD_SAMPLE_H
