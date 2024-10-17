import sys
sys.setrecursionlimit(5000)

import pandas as pd
import matplotlib.pyplot as plt
from tkinter import *
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

url = 'https://docs.google.com/spreadsheets/d/1kwH3bOa-CBZTaXC8mDinpVZriDfw89jEjODG1f2xsZo/export?format=csv&gid=0'

def load_data():
    return pd.read_csv(url)

def display_table(data):
    new_window = Toplevel(root)
    new_window.title("Table View")
    new_window.geometry("1000x400")
    frame = Frame(new_window)
    frame.pack(fill=BOTH, expand=True)
    scrollbar = Scrollbar(frame)
    scrollbar.pack(side=RIGHT, fill=Y)
    tree = ttk.Treeview(frame, yscrollcommand=scrollbar.set, show='headings')
    tree.pack(fill=BOTH, expand=True)
    scrollbar.config(command=tree.yview)
    tree["columns"] = list(data.columns)

    for col in data.columns:
        tree.column(col, anchor=CENTER)
        tree.heading(col, text=col, anchor=CENTER)

    for index, row in data.iterrows():
        tree.insert("", "end", values=list(row))

def plot_graph():
    x = load_data()
    if x.empty:
        return

    new_window = Toplevel(root)
    new_window.title("Plot View")
    new_window.geometry("900x600")
    df_filtered = x[['Date', 'TDS']]
    df_filtered = df_filtered.groupby('Date').mean().reset_index()
    df_filtered.columns = ['Date', 'TDS']
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(df_filtered['Date'], df_filtered['TDS'], marker='o', color='b', linestyle='-', linewidth=2)
    ax.set_title("TDS vs Date", fontsize=16)
    ax.set_xlabel("Date", fontsize=14)
    ax.set_ylabel("TDS", fontsize=14)
    ax.grid(True)

    canvas = FigureCanvasTkAgg(fig, master=new_window)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=BOTH, expand=True)

def show_table():
    x = load_data()
    display_table(x)

def show_high_tds():
    x = load_data()
    display_table(x[x['TDS']>300])

def show_days_water():
    x = load_data()
    display_table(x[x['Water leakage'] == TRUE])

def show_plot():
    plot_graph()

def create_button(parent, text, command):
    button = Button(parent, text=text, command=command, bg="#f7f7f7", fg="#333", font=("Arial", 12, "bold"),
                    relief="flat", bd=0)
    button.config(padx=20, pady=10)
    button.pack(pady=10)
    return button

def update_dashboard():
    data = load_data()
    if not data.empty:
        last_row = data.iloc[-1]
        date_entry.delete(0, END)
        date_entry.insert(0, str(last_row['Date']))
        water_entry.delete(0, END)
        water_entry.insert(0, str(last_row['Water leakage']))
        tds_entry.delete(0, END)
        tds_entry.insert(0, str(last_row['TDS']))
        lvl_entry.delete(0, END)
        lvl_entry.insert(0, str(last_row['Volume']))
    root.after(30000, update_dashboard)


root = Tk()
root.title("Data Visualizations Application")
root.geometry("800x600")
root.configure(bg="#8B0000")

button_frame = Frame(root, bg="#8B0000")
button_frame.pack(pady=20, fill=BOTH, expand=True)

create_button(button_frame, "View Full Table", show_table)
create_button(button_frame, "View Days with dangerous TDS", show_high_tds)
create_button(button_frame, "View Days with water leakage", show_days_water)
create_button(button_frame, "Plot TDS value over Days", show_plot)

dashboard_frame = Frame(root, bg="#8B0000")
dashboard_frame.pack(side=BOTTOM, fill=X)

dashboard_label = Label(dashboard_frame, text="Dashboard", bg="#8B0000", fg="#ffffff", font=("Arial", 14, "bold"))
dashboard_label.pack(pady=5)

dashboard_entries_frame = Frame(dashboard_frame, bg="#8B0000")
dashboard_entries_frame.pack(pady=10)

date_label = Label(dashboard_entries_frame, text="Date: ", bg="#8B0000", fg="#ffffff", font=("Arial", 12))
date_label.grid(row=0, column=0, padx=5)

date_entry = Entry(dashboard_entries_frame, font=("Arial", 12), width=30)
date_entry.grid(row=0, column=1, padx=5)

water_label = Label(dashboard_entries_frame, text="Water leakage: ", bg="#8B0000", fg="#ffffff", font=("Arial", 12))
water_label.grid(row=1, column=0, padx=5)

water_entry = Entry(dashboard_entries_frame, font=("Arial", 12), width=30)
water_entry.grid(row=1, column=1, padx=5)

tds_label = Label(dashboard_entries_frame, text="TDS: ", bg="#8B0000", fg="#ffffff", font=("Arial", 12))
tds_label.grid(row=2, column=0, padx=5)

tds_entry = Entry(dashboard_entries_frame, font=("Arial", 12), width=30)
tds_entry.grid(row=2, column=1, padx=5)

lvl_label = Label(dashboard_entries_frame, text="Water level percent % : ", bg="#8B0000", fg="#ffffff", font=("Arial", 12))
lvl_label.grid(row=3, column=0, padx=5)

lvl_entry = Entry(dashboard_entries_frame, font=("Arial", 12), width=30)
lvl_entry.grid(row=3, column=1, padx=5)

update_dashboard()

root.mainloop()