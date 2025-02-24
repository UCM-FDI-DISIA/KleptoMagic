class EntityStat
{
public:
	EntityStat(float initial);
	~EntityStat();
	void Reset();
	void ChangeAddi(float add);
	void ChangeMulti(float mult);
	float GetStata() { return endStat; }
private:
	float iniStat;
	float endStat;
	float multi;
	float addi;
};

