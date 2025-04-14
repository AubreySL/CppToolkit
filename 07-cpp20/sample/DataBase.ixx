export module database;
import <string>;
import <vector>;
import employee;

namespace Records
{
	const int FirstEmployeeNumber{ 1'000 };

	export class DataBase
	{
	public:
		Employee& addEmployee(const std::string& firstName,
								const std::string& lastName);
		Employee& getEmployee(int employeeNumber);
		Employee& getEmployee(const std::string& firstName,
								const std::string& lastName);
		
		void displayAll() const;
		void displayCurrent() const;
		void displayFormer() const;

	private:
		std::vector<Employee> m_employees;
		int m_nextEmployeeNumber{ FirstEmployeeNumber };
	};

	Employee& DataBase::addEmployee(const std::string& firstName, const std::string& lastName)
	{
		Employee theEmployee{ firstName, lastName };
		theEmployee.setEmployeeNumber(m_nextEmployeeNumber++);
		theEmployee.hire();
		m_employees.push_back(theEmployee);
		return m_employees.back();
	}
}