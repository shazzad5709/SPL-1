class Bank {
	private int bankID;
	private String name;
	private String address;
	private Teller tellers;
	private Customer customers;

	public int getBankID() {
		return bankID;
	}

	public void setBankID(int newBankID) {
		this.bankID = newBankID;
	}

	public String getName() {
		return name;
	}

	public void setName(String newName) {
		this.name = newName;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String newAddress) {
		this.address = newAddress;
	}

	public Teller getTellers() {
		return tellers;
	}

	public void setTellers(Teller newTellers) {
		this.tellers = newTellers;
	}

	public Customer getCustomers() {
		return customers;
	}

	public void setCustomers(Customer newCustomers) {
		this.customers = newCustomers;
	}

}