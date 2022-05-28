class Bank {
	private int bankID;
	private String name;
	private String address;
	private Teller tellers;
	private Customer customers;

	public int getBankID() {
		return bankID;
	}

	public setBankID(int newBankID) {
		this.bankID = newBankID;
	}

	public String getName() {
		return name;
	}

	public setName(String newName) {
		this.name = newName;
	}

	public String getAddress() {
		return address;
	}

	public setAddress(String newAddress) {
		this.address = newAddress;
	}

	public Teller getTellers() {
		return tellers;
	}

	public setTellers(Teller newTellers) {
		this.tellers = newTellers;
	}

	public Customer getCustomers() {
		return customers;
	}

	public setCustomers(Customer newCustomers) {
		this.customers = newCustomers;
	}

}