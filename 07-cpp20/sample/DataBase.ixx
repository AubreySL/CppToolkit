export module database;
import <string>;
import <vector>;
import <stdexcept>;
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
	Employee& DataBase::getEmployee(int employeeNumber)
	{
		for (auto& employee : m_employees)
		{
			if (employee.getEmployeeNumber() == employeeNumber) {
				return employee;
			}
		}
		using namespace std;
		throw logic_error{ "No employee found." };
	}
	Employee& DataBase::getEmployee(const std::string& firstName, const std::string& lastName)
	{
		for (auto& employee : m_employees)
		{
			if (employee.getFirstName()== firstName && employee.getLastName()== lastName) {
				return employee;
			}
		}
		using namespace std;
		throw logic_error{ "No employee found." };
	}

	void DataBase::displayAll() const
	{
		for (const auto& employee : m_employees)
		{
			employee.display();
		}
	}

	void DataBase::displayCurrent() const
	{
		for (const auto& employee : m_employees)
		{
			if (employee.isHired()) { employee.display(); }
		}
	}

	void DataBase::displayFormer() const
	{
		for (const auto& employee : m_employees)
		{
			if (!employee.isHired())
			{ 
				employee.display();
			}
		}
	}

}