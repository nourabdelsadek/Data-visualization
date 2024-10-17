Data Visualizations Application
Overview
This is a Python-based GUI application that visualizes data from a Google Sheets CSV file. The data is loaded dynamically, and the application allows users to perform various operations such as viewing a table, filtering data, and plotting a graph based on TDS (Total Dissolved Solids) values. The application uses Tkinter for the user interface, Pandas for data manipulation, and Matplotlib for plotting.

Features
View Full Table: Displays the entire dataset in a new window using a table format.
View Days with Dangerous TDS: Filters and displays the days where the TDS value exceeds 300, indicating potential water quality issues.
View Days with Water Leakage: Shows only the days when water leakage is recorded.
Plot TDS Value Over Days: Generates a line plot of the average TDS values against time, providing a graphical representation of water quality over time.
Dashboard: Displays the latest data entry in the dataset, including Date, Water Leakage, TDS, and Water Volume percentage.
Libraries Used
Pandas: For data manipulation and loading the CSV data from the Google Sheets URL.
Tkinter: For building the graphical user interface (GUI).
Matplotlib: For plotting graphs inside the application.
ttk: For enhancing the appearance of widgets, specifically the table view.
How It Works
Loading Data: The data is fetched from a Google Sheets CSV link using Pandas (pd.read_csv()).
User Interface: The main application window includes buttons to trigger different actions like viewing the table, filtering data, and plotting graphs.
Plotting and Data Display: Graphs are plotted using Matplotlib, and tables are displayed using ttk.Treeview.
Main Functionalities:
load_data(): Loads the CSV data from the specified URL.
display_table(data): Displays the data in a tabular format within a new window.
plot_graph(): Plots a line graph of TDS values against the date.
update_dashboard(): Automatically updates the dashboard with the most recent data every 30 seconds.
How to Run
Install the required libraries using:
bash
Copy code
pip install pandas matplotlib
Run the Python script.
bash
Copy code
python script.py
The main window will launch with buttons to view data and plots.
CSV Data Format
The data should include the following columns for proper functionality:

Date: The date of the reading.
TDS: The Total Dissolved Solids value.
Water leakage: Boolean indicating if water leakage was detected.
Volume: Water volume percentage.
Customization
You can modify the Google Sheets URL by updating the url variable to load different datasets.
The application is customizable in terms of the columns to filter, plot, and display in the dashboard.
Future Enhancements
Add more visualization types (e.g., bar charts, histograms).
Improve error handling for cases when the dataset is empty or the CSV link is invalid.
Add more filters for data analysis.
