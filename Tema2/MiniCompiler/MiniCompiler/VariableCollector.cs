using System;
using System.Collections.Generic;

public class VariableCollectorVisitor : MiniLangBaseVisitor<object>
{
    public List<string> GlobalVariables = new List<string>();

    public override object VisitVarDeclaration(MiniLangParser.VarDeclarationContext context)
    {
        string type = context.type().GetText();
        string name = context.IDENTIFIER().GetText();
        string value = context.expression()?.GetText() ?? "null";
        GlobalVariables.Add($"{type} {name} = {value}");
        return base.VisitVarDeclaration(context);
    }
}
