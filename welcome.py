import tkinter as tk

name = None

def setName():
    global name
    name = e1.get()
    root.destroy()


def gui():
    global root, e1
    root = tk.Tk()
    root.geometry("300x100")
    root.title("PentagoWelcome")

    l1 = tk.Label(root, text="Your name:")
    e1 = tk.Entry(root)
    b1 = tk.Button(root, text="Start!", command=setName)
    l1.place(x=10, y=10, width=100, height=30)
    e1.place(x=110, y=10, width=180, height=30)
    b1.place(x=100, y=50, width=100, height=30)

    root.mainloop()

def getName():
    gui()
    return name