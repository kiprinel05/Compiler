using System;
using System.Collections.Generic;

public class FunctionCollectorVisitor : MiniLangBaseVisitor<object>
{
    public List<string> Functions = new List<string>();

    public override object VisitFunction(MiniLangParser.FunctionContext context)
    {
        string returnType = context.type().GetText();
        string name = context.IDENTIFIER().GetText();
        string parameters = context.parameterList()?.GetText() ?? "none";
        Functions.Add($"{returnType} {name} ({parameters})");
        return base.VisitFunction(context);
    }
}