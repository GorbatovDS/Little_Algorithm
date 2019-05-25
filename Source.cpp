#include <fstream>
#include <locale>
#include "Branch.h"
#include "Little.h"
#include "CostMatrix.h"

std::string Name_in = "SampleInp.txt";
std::string Name_out = "SampleOut.txt";

int main(int ac, char *av[])
{
	setlocale(LC_ALL, "rus");
	setlocale(LC_NUMERIC, "eng");


	CostMatrix m;

	ifstream inp(Name_in);
	ofstream out(Name_out);
	
	cout << "Данные будут взяты из файла " << Name_in <<
	"\nВыберите один из вариантов:\n"
	"1) Расчет оптимального пути обхода для одного графа\n"
	"2) Расчет оптимального пути обхода для нескольких графов"
	<< endl;

	int choice;

	while (1)
	{
		cin >> choice;
		cout << "Processing...\n";

		if (choice == 1)
		{
			inp >> m;

			Path path;
			CostValue cost = littleSolve(m, path);

			out << m;

			out << "Cost: " << cost << endl
				<< "Path: " << path << endl;

			break;
		}
		else
		{
			if (choice == 2)
			{
				inp >> m;

				

				CostValue cost_sum(0);

				for (size_t i = 0; i < m.mSize(); i++)
				{
					CostMatrix m_else;

					inp >> m_else;
					out << m_else;

					Path path;
					CostValue cost = littleSolve(m_else, path);
					cost_sum += cost;

					out << "Cost: " << cost << endl
						<< "Path: " << path << endl;

					out << "\n\n";
				}

				Path path;
				CostValue cost = littleSolve(m, path);
				//cost += cost_sum;

				out << m;

				out << "Cost: " << cost << endl
					<< "Path: " << path << endl;

				break;
			}
			else
			{
				cout << "Такого варианта не существует.\nПожалуйста введите вариант 1 или 2" << endl;
			}
		}
	}
	

	cout << "Ответ записан в " << Name_out << endl;
	system("pause");
	return 0;
}