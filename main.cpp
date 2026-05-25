#include <iostream>
#include <windows.h> // For color and sound
#include <string>
using namespace std;

int main() {
	const int maxBooks = 100;
	const int maxMembers = 50;
	string bookTitle[maxBooks];
	string bookAuthor[maxBooks];
	string bookCategory[maxBooks];
	float bookPrice[maxBooks];
	int bookID[maxBooks];
	int totalBooks = 0;
	string memberName[maxMembers];
	int memberID[maxMembers];
	char memberType[maxMembers]; // 'S' for student, 'T' for teacher
	int booksIssuedCount[maxMembers] = { 0 }; // Track books issued per member
	int totalMembers = 0;
	int issuedBooks[maxBooks] = { 0 }; // 0 = Available, 1 = Issued (stores member ID)

	while (true) {
		system("cls");
		cout << "\n\n                                                    ====== LIBRARY MANAGEMENT SYSTEM ======\n\n" << endl;
		cout << "1.  Issue Book" << endl;
		cout << "2.  Return Book" << endl;
		cout << "3.  Buy Book" << endl;
		cout << "4.  Register Member" << endl;
		cout << "5.  Update Member Details" << endl;
		cout << "6.  Total Books Issued" << endl;
		cout << "7.  Add Book" << endl;
		cout << "8.  Update Book" << endl;
		cout << "9.  Remove Book" << endl;
		cout << "10. View All Books" << endl;
		cout << "11. Search Book by Title" << endl;
		cout << "12. Search Book by Author" << endl;
		cout << "13. Search Book by Category" << endl;
		cout << "14. Exit\n" << endl;
		cout << "Enter your choice (1-14): ";
		int choice;
		if (!(cin >> choice) || choice < 1 || choice > 14) {
			cin.clear(); // Clear error state
			cin.ignore(10000, '\n'); // Discard invalid input
			cout << "Invalid input! Please enter a number (1-14)." << endl;
			Beep(500, 500); // Error sound
			Sleep(1000); // 1 sec delay
			continue;
		}
		cin.ignore(10000, '\n'); // Clear the newline after number input

		if (choice == 1) { // Issue Book
			cout << "Enter Member ID: ";
			int memberId;
			if (!(cin >> memberId)) {
				cout << "Invalid Member ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			// Find member
			int memberIndex = -1;
			char type = ' ';
			for (int i = 0; i < totalMembers; i++) {
				if (memberID[i] == memberId) {
					memberIndex = i;
					type = memberType[i];
					break;
				}
			}

			if (memberIndex == -1) {
				cout << "Member not found!" << endl;
				Beep(500, 500);
				cout << "\nPress Enter to continue...";
				cin.ignore(10000, '\n');
				continue;
			}

			// Check issuing limit
			if ((type == 'S' && booksIssuedCount[memberIndex] >= 2) ||
				(type == 'T' && booksIssuedCount[memberIndex] >= 3)) {
				cout << "Issue limit reached! ";
				if (type == 'S') cout << "Students can issue max 2 books." << endl;
				else cout << "Teachers can issue max 3 books." << endl;
				Beep(500, 500);
				cout << "\nPress Enter to continue...";
				cin.ignore(10000, '\n');
				continue;
			}

			cout << "Enter Book ID to issue: ";
			int bookId;
			if (!(cin >> bookId)) {
				cout << "Invalid Book ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			bool bookFound = false;
			for (int i = 0; i < totalBooks; i++) {
				if (bookID[i] == bookId) {
					bookFound = true;
					if (issuedBooks[i] == 0) {
						issuedBooks[i] = memberId; // Store member ID who issued the book
						booksIssuedCount[memberIndex]++;
						cout << "Book issued successfully to "
							<< (type == 'S' ? "Student" : "Teacher")
							<< " " << memberName[memberIndex] << "!" << endl;
						Beep(1000, 200);
					}
					else {
						cout << "Book is already issued!" << endl;
						Beep(500, 500);
					}
					break;
				}
			}

			if (!bookFound) {
				cout << "Book not found!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 2) { // Return Book
			cout << "Enter Book ID to return: ";
			int bookId;
			if (!(cin >> bookId)) {
				cout << "Invalid Book ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			bool bookFound = false;
			for (int i = 0; i < totalBooks; i++) {
				if (bookID[i] == bookId) {
					bookFound = true;
					if (issuedBooks[i] != 0) {
						// Find member to decrement their issued count
						int memberId = issuedBooks[i];
						for (int j = 0; j < totalMembers; j++) {
							if (memberID[j] == memberId) {
								booksIssuedCount[j]--;
								break;
							}
						}
						issuedBooks[i] = 0;
						cout << "Book returned successfully!" << endl;
						Beep(1000, 200);
					}
					else {
						cout << "Book was not issued!" << endl;
						Beep(500, 500);
					}
					break;
				}
			}

			if (!bookFound) {
				cout << "Book not found!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 3 || choice == 7) { // Buy Book or Add Book
			if (totalBooks < maxBooks) {
				cout << "Enter Book ID: ";
				int id;
				if (!(cin >> id)) {
					cout << "Invalid Book ID! Please enter a numeric value." << endl;
					Beep(500, 500);
					cin.clear();
					cin.ignore(10000, '\n');
					Sleep(1000);
					continue;
				}
				cin.ignore(10000, '\n');
				bookID[totalBooks] = id;

				string input;
				bool valid = false;

				// Title validation
				do {
					cout << "Enter Title (letters, numbers, and spaces only): ";
					getline(cin, input);
					valid = true;
					for (char c : input) {
						if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
							(c >= '0' && c <= '9') || c == ' ')) {
							valid = false;
							cout << "Invalid Title! Only letters, numbers and spaces allowed." << endl;
							Beep(500, 500);
							Sleep(500);
							break;
						}
					}
				} while (!valid);
				bookTitle[totalBooks] = input;

				// Author validation (letters and spaces only)
				do {
					cout << "Enter Author (letters and spaces only): ";
					getline(cin, input);
					valid = true;
					for (char c : input) {
						if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
							valid = false;
							cout << "Invalid Author! Only letters and spaces allowed." << endl;
							Beep(500, 500);
							Sleep(500);
							break;
						}
					}
				} while (!valid);
				bookAuthor[totalBooks] = input;

				// Category validation
				do {
					cout << "Enter Category (letters, numbers, spaces, and underscores only): ";
					getline(cin, input);
					valid = true;
					for (char c : input) {
						if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
							(c >= '0' && c <= '9') || c == ' ' || c == '_')) {
							valid = false;
							cout << "Invalid Category! Only letters, numbers, spaces and underscores allowed." << endl;
							Beep(500, 500);
							Sleep(500);
							break;
						}
					}
				} while (!valid);
				bookCategory[totalBooks] = input;

				// Price validation
				float price;
				while (true) {
					cout << "Enter Price: $";
					if (!(cin >> price)) {
						cout << "Invalid Price! Please enter a numeric value." << endl;
						Beep(500, 500);
						cin.clear();
						cin.ignore(10000, '\n');
						Sleep(500);
					}
					else {
						cin.ignore(10000, '\n');
						break;
					}
				}
				bookPrice[totalBooks] = price;
				issuedBooks[totalBooks] = 0;
				totalBooks++;
				cout << "Book added successfully!" << endl;
				Beep(1000, 200);
			}
			else {
				cout << "Library is full! Cannot add more books." << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 4) { // Register Member
			if (totalMembers < maxMembers) {
				cout << "Enter Member ID: ";
				int id;
				if (!(cin >> id)) {
					cout << "Invalid Member ID! Please enter a numeric value." << endl;
					Beep(500, 500);
					cin.clear();
					cin.ignore(10000, '\n');
					Sleep(1000);
					continue;
				}
				cin.ignore(10000, '\n');

				// Check if member ID already exists
				bool idExists = false;
				for (int i = 0; i < totalMembers; i++) {
					if (memberID[i] == id) {
						idExists = true;
						break;
					}
				}

				if (idExists) {
					cout << "Member ID already exists!" << endl;
					Beep(500, 500);
					cout << "\nPress Enter to continue...";
					cin.ignore(10000, '\n');
					continue;
				}

				memberID[totalMembers] = id;

				// Get member type
				char type;
				while (true) {
					cout << "Enter Member Type (S for Student, T for Teacher): ";
					cin >> type;
					cin.ignore(10000, '\n');
					type = toupper(type);
					if (type == 'S' || type == 'T') {
						break;
					}
					cout << "Invalid type! Please enter S or T." << endl;
					Beep(500, 500);
				}
				memberType[totalMembers] = type;

				// Member name validation (letters and spaces only)
				string name;
				bool valid = false;
				do {
					cout << "Enter Name (letters and spaces only): ";
					getline(cin, name);
					valid = true;
					for (char c : name) {
						if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
							valid = false;
							cout << "Invalid Name! Only letters and spaces allowed." << endl;
							Beep(500, 500);
							Sleep(500);
							break;
						}
					}
				} while (!valid);

				memberName[totalMembers] = name;
				booksIssuedCount[totalMembers] = 0;
				totalMembers++;
				cout << "Member registered successfully as "
					<< (type == 'S' ? "Student" : "Teacher") << "!" << endl;
				Beep(1000, 200);
			}
			else {
				cout << "Member limit reached!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 5) { // Update Member Details
			int id;
			cout << "Enter Member ID to update: ";
			if (!(cin >> id)) {
				cout << "Invalid Member ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			bool found = false;
			for (int i = 0; i < totalMembers; i++) {
				if (memberID[i] == id) {
					found = true;

					// Member name validation (letters and spaces only)
					string name;
					bool valid = false;
					do {
						cout << "Enter new name (letters and spaces only): ";
						getline(cin, name);
						valid = true;
						for (char c : name) {
							if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
								valid = false;
								cout << "Invalid Name! Only letters and spaces allowed." << endl;
								Beep(500, 500);
								Sleep(500);
								break;
							}
						}
					} while (!valid);

					memberName[i] = name;

					// Update member type
					char type;
					while (true) {
						cout << "Enter new Member Type (S for Student, T for Teacher): ";
						cin >> type;
						cin.ignore(10000, '\n');
						type = toupper(type);
						if (type == 'S' || type == 'T') {
							break;
						}
						cout << "Invalid type! Please enter S or T." << endl;
						Beep(500, 500);
					}
					memberType[i] = type;

					cout << "Member details updated!" << endl;
					Beep(1000, 200);
					break;
				}
			}

			if (!found) {
				cout << "Member not found!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 6) { // Total Books Issued
			int count = 0;
			for (int i = 0; i < totalBooks; i++) {
				if (issuedBooks[i] != 0) count++;
			}
			cout << "Total books issued: " << count << endl;
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 8) { // Update Book
			cout << "Enter Book ID to update: ";
			int id;
			if (!(cin >> id)) {
				cout << "Invalid Book ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			bool found = false;
			for (int i = 0; i < totalBooks; i++) {
				if (bookID[i] == id) {
					found = true;
					string input;
					bool valid = false;

					// Title validation
					do {
						cout << "Enter new Title (letters, numbers, and spaces only): ";
						getline(cin, input);
						valid = true;
						for (char c : input) {
							if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
								(c >= '0' && c <= '9') || c == ' ')) {
								valid = false;
								cout << "Invalid Title! Only letters, numbers and spaces allowed." << endl;
								Beep(500, 500);
								Sleep(500);
								break;
							}
						}
					} while (!valid);
					bookTitle[i] = input;

					// Author validation (letters and spaces only)
					do {
						cout << "Enter new Author (letters and spaces only): ";
						getline(cin, input);
						valid = true;
						for (char c : input) {
							if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
								valid = false;
								cout << "Invalid Author! Only letters and spaces allowed." << endl;
								Beep(500, 500);
								Sleep(500);
								break;
							}
						}
					} while (!valid);
					bookAuthor[i] = input;

					// Category validation
					do {
						cout << "Enter new Category (letters, numbers, spaces, and underscores only): ";
						getline(cin, input);
						valid = true;
						for (char c : input) {
							if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
								(c >= '0' && c <= '9') || c == ' ' || c == '_')) {
								valid = false;
								cout << "Invalid Category! Only letters, numbers, spaces and underscores allowed." << endl;
								Beep(500, 500);
								Sleep(500);
								break;
							}
						}
					} while (!valid);
					bookCategory[i] = input;

					// Price validation
					float price;
					while (true) {
						cout << "Enter new Price: $";
						if (!(cin >> price)) {
							cout << "Invalid Price! Please enter a numeric value." << endl;
							Beep(500, 500);
							cin.clear();
							cin.ignore(10000, '\n');
							Sleep(500);
						}
						else {
							cin.ignore(10000, '\n');
							break;
						}
					}
					bookPrice[i] = price;
					cout << "Book updated successfully!" << endl;
					Beep(1000, 200);
					break;
				}
			}

			if (!found) {
				cout << "Book not found!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 9) { // Remove Book
			cout << "Enter Book ID to remove: ";
			int id;
			if (!(cin >> id)) {
				cout << "Invalid Book ID! Please enter a numeric value." << endl;
				Beep(500, 500);
				cin.clear();
				cin.ignore(10000, '\n');
				Sleep(1000);
				continue;
			}
			cin.ignore(10000, '\n');

			bool found = false;
			for (int i = 0; i < totalBooks; i++) {
				if (bookID[i] == id) {
					found = true;
					// If book is issued, decrement member's issued count
					if (issuedBooks[i] != 0) {
						int memberId = issuedBooks[i];
						for (int j = 0; j < totalMembers; j++) {
							if (memberID[j] == memberId) {
								booksIssuedCount[j]--;
								break;
							}
						}
					}

					// Remove the book
					for (int j = i; j < totalBooks - 1; j++) {
						bookID[j] = bookID[j + 1];
						bookTitle[j] = bookTitle[j + 1];
						bookAuthor[j] = bookAuthor[j + 1];
						bookCategory[j] = bookCategory[j + 1];
						bookPrice[j] = bookPrice[j + 1];
						issuedBooks[j] = issuedBooks[j + 1];
					}
					totalBooks--;
					cout << "Book removed successfully!" << endl;
					Beep(1000, 200);
					break;
				}
			}

			if (!found) {
				cout << "Book not found!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 10) { // View All Books
			cout << "\n===== ALL BOOKS =====" << endl;
			for (int i = 0; i < totalBooks; i++) {
				cout << "ID: " << bookID[i] << " | Title: " << bookTitle[i]
					<< " | Author: " << bookAuthor[i] << " | Category: "
					<< bookCategory[i] << " | Price: $" << bookPrice[i]
					<< " | Status: " << (issuedBooks[i] != 0 ? "Issued" : "Available");
				if (issuedBooks[i] != 0) {
					// Find member name
					string issuerName = "Unknown";
					for (int j = 0; j < totalMembers; j++) {
						if (memberID[j] == issuedBooks[i]) {
							issuerName = memberName[j];
							break;
						}
					}
					cout << " (Issued to: " << issuerName << ")";
				}
				cout << endl;
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 11) { // Search by Title
			string title;
			cout << "Enter book title to search: ";
			getline(cin, title);
			bool found = false;
			cout << "\n===== SEARCH RESULTS =====" << endl;
			for (int i = 0; i < totalBooks; i++) {
				if (bookTitle[i] == title) {
					found = true;
					cout << "ID: " << bookID[i] << " | Title: " << bookTitle[i]
						<< " | Author: " << bookAuthor[i] << " | Category: "
						<< bookCategory[i] << " | Price: $" << bookPrice[i]
						<< " | Status: " << (issuedBooks[i] != 0 ? "Issued" : "Available");
					if (issuedBooks[i] != 0) {
						// Find member name
						string issuerName = "Unknown";
						for (int j = 0; j < totalMembers; j++) {
							if (memberID[j] == issuedBooks[i]) {
								issuerName = memberName[j];
								break;
							}
						}
						cout << " (Issued to: " << issuerName << ")";
					}
					cout << endl;
				}
			}
			if (!found) {
				cout << "No books found with this title!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 12) { // Search by Author
			string author;
			cout << "Enter author name to search: ";
			getline(cin, author);
			bool found = false;
			cout << "\n===== SEARCH RESULTS =====" << endl;
			for (int i = 0; i < totalBooks; i++) {
				if (bookAuthor[i] == author) {
					if (!found) {
						cout << "Author: " << author << endl;
						found = true;
					}
					cout << "ID: " << bookID[i] << " | Title: " << bookTitle[i]
						<< " | Category: " << bookCategory[i] << " | Price: $" << bookPrice[i]
						<< " | Status: " << (issuedBooks[i] != 0 ? "Issued" : "Available");
					if (issuedBooks[i] != 0) {
						// Find member name
						string issuerName = "Unknown";
						for (int j = 0; j < totalMembers; j++) {
							if (memberID[j] == issuedBooks[i]) {
								issuerName = memberName[j];
								break;
							}
						}
						cout << " (Issued to: " << issuerName << ")";
					}
					cout << endl;
				}
			}
			if (!found) {
				cout << "No books found by this author!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 13) { // Search by Category
			string category;
			cout << "Enter category to search: ";
			getline(cin, category);
			bool found = false;
			cout << "\n===== SEARCH RESULTS =====" << endl;
			for (int i = 0; i < totalBooks; i++) {
				if (bookCategory[i] == category) {
					found = true;
					cout << "ID: " << bookID[i] << " | Title: " << bookTitle[i]
						<< " | Author: " << bookAuthor[i] << " | Category: "
						<< bookCategory[i] << " | Price: $" << bookPrice[i]
						<< " | Status: " << (issuedBooks[i] != 0 ? "Issued" : "Available");
					if (issuedBooks[i] != 0) {
						// Find member name
						string issuerName = "Unknown";
						for (int j = 0; j < totalMembers; j++) {
							if (memberID[j] == issuedBooks[i]) {
								issuerName = memberName[j];
								break;
							}
						}
						cout << " (Issued to: " << issuerName << ")";
					}
					cout << endl;
				}
			}
			if (!found) {
				cout << "No books found in this category!" << endl;
				Beep(500, 500);
			}
			cout << "\nPress Enter to continue...";
			cin.ignore(10000, '\n');
		}
		else if (choice == 14) { // Exit
			cout << "Are you sure you want to exit? (y/n): ";
			char confirm;
			cin >> confirm;
			cin.ignore(10000, '\n');
			if (confirm == 'y' || confirm == 'Y') {
				cout << "Exiting the program..." << endl;
				Beep(1000, 500);
				break;
			}
		}
	}
	return 0;
}