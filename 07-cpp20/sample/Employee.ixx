export module employee;
import <string>;
import <iostream>;
import <format>;

namespace Records {
	const int DefaultStartingSalry{ 30'000 };
	export const int DefaultRaiseAndDemeritAmount{ 1'000 };

	export class Employee 
	{
	public:
		Employee(const std::string& firstName,
			const std::string& lastName);

		void promote(int raiseAmount = DefaultRaiseAndDemeritAmount);
		void demote(int demeritAmount = DefaultRaiseAndDemeritAmount);
		void hire();
		void fire();
		void display() const;

		void setFirstName(const std::string& firstName);
		const std::string& getFirstName() const;

		void setLastName(const std::string& lastName);
		const std::string& getLastName() const;

		void setEmployeeNumber(int employeeNumber);
		int getEmployeeNumber() const;

		void setSalary(int newSalary);
		int getSalary()	const;

		bool isHired() const;

	private:
		std::string m_firstName;
		std::string m_lastName;
		int m_employeeNumber{ -1 };
		int m_salary{ DefaultStartingSalry };
		bool m_hired{ false };

	};

	//It seems that vs don't support define module separately
	//implemention of clasee Employee
	Employee::Employee(const std::string& firstName, const std::string& lastName)
		:m_firstName{ firstName }, m_lastName{ lastName }
	{
	}

	void Employee::promote(int raiseAmount)
	{
		setSalary(getSalary()+ raiseAmount);
	}

	void Employee::demote(int demeritAmount)
	{
		setSalary(getSalary() - demeritAmount);
	}

	void Employee::hire()
	{
		m_hired = true;
	}

	void Employee::fire()
	{
		m_hired = false;
	}

	void Employee::display() const
	{
		using namespace std;
		cout << format("Employee: {}, {}", getLastName(), getFirstName()) << endl;
		cout << "------------------------------------" << endl;
		cout << (isHired() ? "Current Employee" : "Former Employee") << endl;
		cout << format("Employee Number:{}", getEmployeeNumber()) << endl;
		cout << format("Salary:${}", getSalary()) << endl;
		cout << endl;
	}

	void Employee::setFirstName(const std::string& firstName)
	{
		m_firstName = firstName;
	}

	const std::string& Employee::getFirstName() const
	{
		return m_firstName;
	}

	void Employee::setLastName(const std::string& lastName)
	{
		m_lastName = lastName;
	}

	const std::string& Employee::getLastName() const
	{
		return m_lastName;
	}

	void Employee::setEmployeeNumber(int employeeNumber)
	{
		m_employeeNumber = employeeNumber;
	}

	int Employee::getEmployeeNumber() const
	{
		return m_employeeNumber;
	}

	void Employee::setSalary(int newSalary)
	{
		m_salary = newSalary;
	}

	int Employee::getSalary() const
	{
		return m_salary;
	}

	bool Employee::isHired() const
	{
		return m_hired;
	}


	


}
